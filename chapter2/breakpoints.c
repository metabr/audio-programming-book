#include <breakpoints.h>
#include <stdlib.h>

BREAKPOINT maxpoint(const BREAKPOINT* points,long npoints)
{
        int i;
        BREAKPOINT point;

        point.time = 0.0;
        point.value = 0.0;

        for(i=0;i < npoints;i++){
                if(point.value < points[i].value){
                        point.value = points[i].value;
                        point.time = points[i].time;
                }
        }
        return point;
}

BREAKPOINT* get_breakpoints(FILE* fp, long* psize)
{
        int got;

        long npoints = 0, size = NPOINTS;
        double lasttime    = 0.0;
        BREAKPOINT* points = NULL;
        char line[LINELENGTH];

        if(fp==NULL)
                return NULL;
        points = (BREAKPOINT*) malloc(sizeof(BREAKPOINT) * size);
        if(points==NULL)
                return NULL;

        while(fgets(line,80,fp)){
                if((got = sscanf(line, "%lf%lf", &points[npoints].time,&points[npoints].value) )<0)
                        continue;                         /* empty line */
                if(got==0){
                        printf("Line %ld has non-numeric data\n",npoints+1);
                        break;
                }
                if(got==1){
                        printf("Incomplete breakpoint found at point %ld\n",npoints+1);
                        break;
                }
                if(points[npoints].time < lasttime){
                        printf("error in breakpoint data at point %ld: time not increasing\n", npoints+1);
                        break;
                }
                lasttime = points[npoints].time;
                if(++npoints == size){
                        BREAKPOINT* tmp;
                        size += NPOINTS;
                        tmp = (BREAKPOINT*) realloc(points, sizeof(BREAKPOINT) * size);
                        if(tmp == NULL) {       /* too bad! */
                                /* have to release the memory, and return NULL to caller */
                                npoints = 0;
                                free(points);
                                points = NULL;
                                break;
                        }
                        points = tmp;
                }
        }
        if(npoints)
                *psize = npoints;
        return points;
}
