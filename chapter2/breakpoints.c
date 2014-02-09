#include <breakpoints.h>
#include <stdlib.h>

BREAKPOINT maxpoint(const BREAKPOINT * points, long npoints)
{
	int i;
	BREAKPOINT point;

	point.time = 0.0;
	point.value = 0.0;

	for (i = 0; i < npoints; i++) {
		if (point.value < points[i].value) {
			point.value = points[i].value;
			point.time = points[i].time;
		}
	}
	return point;
}

BREAKPOINT *get_breakpoints(FILE * fp, unsigned long *psize)
{
	int got;

	long npoints = 0, size = NPOINTS;
	double lasttime = 0.0;
	BREAKPOINT *points = NULL;
	char line[LINELENGTH];

	if (fp == NULL)
		return NULL;
	points = (BREAKPOINT *) malloc(sizeof(BREAKPOINT) * size);
	if (points == NULL)
		return NULL;

	while (fgets(line, 80, fp)) {
		if ((got =
		     sscanf(line, "%lf%lf", &points[npoints].time,
			    &points[npoints].value)) < 0)
			continue;	/* empty line */
		if (got == 0) {
			printf("Line %ld has non-numeric data\n",
			       npoints + 1);
			break;
		}
		if (got == 1) {
			printf
			    ("Incomplete breakpoint found at point %ld\n",
			     npoints + 1);
			break;
		}
		if (points[npoints].time < lasttime) {
			printf
			    ("error in breakpoint data at point %ld: time not increasing\n",
			     npoints + 1);
			break;
		}
		lasttime = points[npoints].time;
		if (++npoints == size) {
			BREAKPOINT *tmp;
			size += NPOINTS;
			tmp =
			    (BREAKPOINT *) realloc(points,
						   sizeof(BREAKPOINT) *
						   size);
			if (tmp == NULL) {	/* too bad! */
				/* have to release the memory, and return NULL to caller */
				npoints = 0;
				free(points);
				points = NULL;
				break;
			}
			points = tmp;
		}
	}
	if (npoints)
		*psize = npoints;
	return points;
}

int inrange(const BREAKPOINT * points, double minval, double maxval, unsigned long npoints)
{
	unsigned long i;
	int range_OK = 1;

	for (i = 0; i < npoints; i++) {
		if (points[i].value < minval || points[i].value > maxval) {
			range_OK = 0;
			break;
		}
	}

	return range_OK;
}

double val_at_brktime(const BREAKPOINT* points, unsigned long npoints, double time)
{
	unsigned long i;
	BREAKPOINT left, right;
	double frac, val, width;

	for (i = 1; i < npoints; i++) {
		if (time <= points[i].time)
			break;
	}

	if (i == npoints)
		return points[i-1].value;

	left = points[i-1];
	right = points[i];

	width = right.time - left.time;
	if (width == 0.0)
		return right.value;

	frac = (time - left.time) / width;
	val = left.value + ((right.value - left.value) * frac);

	return val;
}

BRKSTREAM *bps_newstream(FILE *fp, unsigned long srate, unsigned long *size)
{
	BRKSTREAM *stream;
	BREAKPOINT *points;
	unsigned long npoints;

	if (srate == 0) {
		printf("Error 1\n");
		return NULL;
	}

	stream = malloc(sizeof(*stream));
	if (stream == NULL) {
		printf("Error 2\n");
		return NULL;
	}

	points = get_breakpoints(fp, &npoints);
	if (points == NULL) {
		printf("Error 3\n");
		free(stream);
		return NULL;
	}

	if (npoints < 2) {
		printf("Error 4\n");
		free(stream);
		return NULL;
	}

	/* init stream object */
	stream->points = points;
	stream->npoints = npoints;
	/* counters */
	stream->cur_pos = 0.0;
	stream->ileft = 0;
	stream->iright = 1;
	stream->incr = 1.0 / srate;
	/* first span */
	stream->leftpoint = stream->points[stream->ileft];
	stream->rightpoint = stream->points[stream->iright];
	stream->width = stream->rightpoint.time - stream->leftpoint.time;
	stream->height = stream->rightpoint.value - stream->leftpoint.value;
	stream->more_points = 1;

	if (size)
		*size = stream->npoints;

	return stream;
}

void bps_freepoints(BRKSTREAM *stream)
{
	if (stream && stream->points) {
		free(stream->points);
		stream->points = NULL;
	}
}

double bps_tick(BRKSTREAM *stream)
{
	double thisval, frac;

	/* beyound the end of brkdata */
	if (stream->more_points == 0)
		return stream->rightpoint.value;
	if (stream->width == 0.0)
		return stream->rightpoint.value;
	else {
		/* get value from this span using linear interpolation */
		frac = (stream->cur_pos - stream->leftpoint.time) / stream->width;
		thisval = stream->leftpoint.value + (stream->height * frac);
	}

	stream->cur_pos += stream->incr;
	if (stream->cur_pos > stream->rightpoint.time) {
		/* need to go next span? */
		stream->ileft++;
		stream->iright++;
		if (stream->iright < stream->npoints) {
			stream->leftpoint = stream->points[stream->ileft];
			stream->rightpoint = stream->points[stream->iright];
			stream->width = stream->rightpoint.time - stream->leftpoint.time;
			stream->height = stream->rightpoint.value - stream->leftpoint.value;
		} else {
			stream->more_points = 0;
		}
	}

	return thisval;
}

int bps_getminmax(BRKSTREAM *stream, double *outmin, double *outmax)
{
	double value, min, max;

	if (stream == NULL)
		return -1;

	min = stream->points[0].value;
	max = stream->points[0].value;

	for (int i = 0; i < stream->npoints; i++) {
		value = stream->points[i].value;
		min = value < min ? value : min;
		max = value > max ? value : max;
	}

	*outmin = min;
	*outmax = max;

	return 0;
}
