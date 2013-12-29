#include "mainwindow.h"
#include "editwindow.h"
#include "matrixmode.h"
#include "rawmode.h"
#include "ui_mainwindow.h"
#include "cond.h"
#include "define.h"
#include "morecond.h"
#include <sys/time.h>

//constructor
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    //setup the ui
    ui->setupUi(this);
    //set a fixed size for the window
    this->setFixedSize(670, 600);

    //set all the MIDI filters to 0
    ccFilt = 0;
    noteFilt = 0;
    polyFilt = 0;
    chanFilt = 0;
    progFilt = 0;
    pitchFilt = 0;
    sysFilt = 0;
    realFilt = 0;

    //set all bools to 0
    inOpen = 0;
    outOpen = 0;
    mThru = 0;
    condClockOn = 0;
    dispMidi = 0;
    matrixMode = 0;
    updating = 0;

    //create a timer for MIDI
    mTimer = new QTimer(this);
    mTimerOn = 0;

    initMidi();         //setup MIDI
    checkAllFilters();  //check the filter boxes
    startMidiTimer();   //start the MIDI timer
    makeConnections();  //all of the SIGNAL connections
}

void MainWindow::makeConnections()
{
    //the kill pushbutton is a temporary button to kill the MIDI timer.
    //when MIDI is being read in too quickly by a device sending a ton of MIDI
    //data, some functions cause the application to choke up and crash.
    connect(ui->pbKill, SIGNAL(clicked()), this, SLOT(stopMidiTimer()));

    //connect MIDI input and output combo boxes
    connect(ui->cbMidiIn, SIGNAL(currentIndexChanged(int)), this, SLOT(changeMidiIn()));
    connect(ui->cbMidiOut, SIGNAL(currentIndexChanged(int)), this, SLOT(changeMidiOut()));

    //connect filter check boxes
    connect(ui->cbControllerChange, SIGNAL(stateChanged(int)), this, SLOT(fControllerChange()));
    connect(ui->cbMonoAftertouch, SIGNAL(stateChanged(int)), this, SLOT(fMonoAftertouch()));
    connect(ui->cbNote, SIGNAL(stateChanged(int)), this, SLOT(fNote()));
    connect(ui->cbPitchBend, SIGNAL(stateChanged(int)), this, SLOT(fPitchBend()));
    connect(ui->cbPolyAftertouch, SIGNAL(stateChanged(int)), this, SLOT(fPolyAftertouch()));
    connect(ui->cbProgramChange, SIGNAL(stateChanged(int)), this, SLOT(fProgramChange()));
    connect(ui->cbSysEx, SIGNAL(stateChanged(int)), this, SLOT(fSysEx()));
    connect(ui->cbRealTime, SIGNAL(stateChanged(int)), this, SLOT(fRealTime()));

    //connect mode change combo box
    connect(ui->cbMode, SIGNAL(currentIndexChanged(int)), this, SLOT(modeChange()));

    //connect open conduct file button
    connect(ui->pbConductOpen, SIGNAL(clicked()), this, SLOT(openFile()));

    //connect compile button
    connect(ui->pbCompile, SIGNAL(clicked()), this, SLOT(compile()));

    //connect edit conduct file button
    connect(ui->pbEditFile, SIGNAL(clicked()), this, SLOT(showEditWindow()));

    //connect update devices button
    connect(ui->pbUpdateDevices, SIGNAL(clicked()), this, SLOT(updateDevices()));

    //connect thru check box
    connect(ui->cbThru, SIGNAL(stateChanged(int)), this, SLOT(thru()));

    //connect start conducting button
    connect(ui->pbStartCond, SIGNAL(clicked()), this, SLOT(startCond()));
    connect(ui->pbStopCond, SIGNAL(clicked()), this, SLOT(stopCond()));

    //connect display MIDI check box
    connect(ui->cbDisplayMidi, SIGNAL(stateChanged(int)), this, SLOT(displayMidi()));

    //connect raw and matrix mode push buttons
    connect(ui->pbMatrixMode, SIGNAL(clicked()), this, SLOT(startMatrixMode()));
    connect(ui->pbRawMode, SIGNAL(clicked()), this, SLOT(startRawMode()));
}

//this function checks to see if any MIDI streams are open and closes it if it is
void MainWindow::closeMidi()
{
    if(inOpen)
    {
        Pm_Close(inStream);
        inOpen = 0;
    }
    if(outOpen)
    {
        Pm_Close(outStream);
        outOpen = 0;
    }
}

//this function checks to see if the MIDI streams are closed and opens them if they are
void MainWindow::openMidi()
{
    if(!inOpen)
    {
        Pm_OpenInput(&inStream, inDev, NULL, 512, NULL, NULL);
        if(!mTimerOn) startMidiTimer();
        inOpen = 1;
    }
    if(!outOpen)
    {
        Pm_OpenOutput(&outStream, outDev, NULL, 512, NULL, NULL, 0);
        if(!mTimerOn) startMidiTimer();
        outOpen = 1;
    }
}

//this function opens up the MatrixMode window
void MainWindow::startMatrixMode()
{
    if(mTimerOn) stopMidiTimer();
    closeMidi();

    matrixMode = 1;

    MatrixMode* tlMatrix = new MatrixMode(this, (int)ui->cbMidiIn->currentIndex(), (int)ui->cbMidiOut->currentIndex() + totalInDevices);
    tlMatrix->exec();
    delete tlMatrix;

    matrixMode = 0;
    openMidi();
}

void MainWindow::startRawMode()
{
    //clear buffers
    while (Pm_Poll(inStream))
    {
        Pm_Read(inStream, bufferIn, 1);
    }

    if(mTimerOn) stopMidiTimer();
    closeMidi();
    Pm_Terminate();

    //Pm_SetFilter(inStream, PM_)

    tlRaw = new RawMode(this, inDev, outDev);
    //tlRaw = (RawMode*)malloc(sizeof(tlRaw));
    tlRaw->exec();
    delete tlRaw;

    Pm_Initialize();
    openMidi();

}

void MainWindow::displayMidi()
{
    if(ui->cbDisplayMidi->checkState()) dispMidi = 1;
    else dispMidi = 0;
}

void MainWindow::stopCond()
{
    exitflag=1;
    ui->lvCondDisplay->addItem("Exiting");
    ui->lvCondDisplay->scrollToBottom();
    ui->lvMessages->insertItem(0, "Conductor program terminated");
}

void MainWindow::startCond()
{
    if(!condClockOn)
    {
        ui->lvMessages->insertItem(0, tr("Starting conductor program"));
        setdevices();
        startmidi();

        gettimeofday(&tb,NULL);
        starttime=tb.tv_sec;         //set starttime register

        initialization();

        switch(modee)
        {
        case 0:
            ui->lvMessages->insertItem(0, "Running in conduct mode");
            break;
        case 1:
            ui->lvMessages->insertItem(0, "Running in runs mode");
            break;
        case 2:
            ui->lvMessages->insertItem(0, "Running in rhymicon mode");
            break;
        default:
            ui->lvMessages->insertItem(0, "Mode error!");
            break;
        }

        condClockOn = 1;

        QTimer *timer = new QTimer(this);
        connect(timer, SIGNAL(timeout()), this, SLOT(condClock()));
        timer->start(0);
    }
}

void MainWindow::condClock()
{
    if(condClockOn)
    {
        gettimeofday(&tb,NULL);
        t_time=(((tb.tv_sec-starttime)*4000)+(tb.tv_usec/250)); //update conductor program clock

        PmError status, length;

        status = Pm_Poll(inStream);
        if (status == TRUE)
        {
            length = Pm_Read(inStream,bufferIn,1);
            if (length > 0)
            {
                midibyte0 = (unsigned char)Pm_MessageStatus(bufferIn[0].message);
                midibyte1 = (unsigned char)Pm_MessageData1(bufferIn[0].message);
                midibyte2 = (unsigned char)Pm_MessageData2(bufferIn[0].message);
                midiparser(midibyte0, midibyte1, midibyte2);
            }
        }

        if(playmode)
        {
            if(modee==0)playscorefile();
            if(modee==1)playruns();
            if(modee==2)playrhythmicon();
        }//end if(playmode)

        if(exitflag==1)
            condClockOn = 0;        
    }
}

void MainWindow::initialization()
{
    startmeasure=0;
    initializecond();
}

void MainWindow::midiparser(unsigned char byte0, unsigned char byte1, unsigned char byte2)
{
    unsigned short xxx,arrayindex;
    int ii;
    float zxcorrection,zycorrection;
    arrayindex=(int)byte0 & 0x0f;
    xxx=((int)byte2<<7)+(int)byte1;
    array[arrayindex]=(float)xxx;
    intarray[arrayindex]=xxx;

    if(arrayindex==8)
    {
        buttons=byte1 & 0x1f;if(buttons!=0){buttoncommand |= buttons;}if((buttoncommand!=0) && (buttons==0)){processbuttoncommand();}

        feet=byte2;if(feet!=0){feetcommand |= feet;}if((feetcommand!=0) && (feet==0)){processfeetcommand();}
        xc1=(array[7]/(array[7]+array[1]))*153-10;  if(xc1>127)xc1=127;if(xc1<0)xc1=0;
        yc1=(array[5]/(array[5]+array[3]))*163-15;  if(yc1>127)yc1=127;if(yc1<0)yc1=0;
        zc1p=zc1;
        zxcorrection=(xc1-64)*4.0; if(zxcorrection<0)zxcorrection= -zxcorrection;
        zycorrection=(yc1-64)*8.0; if(zycorrection<0)zycorrection= -zycorrection;
        zc1=array[7]+array[1]+array[5]+array[3]+zxcorrection+zycorrection;

        zc2=array[6]+array[0]+array[4]+array[2];
        if(zc2>200)
        {
            xc2=(array[6]/(array[6]+array[0]))*161-23;  if(xc2>127)xc2=127;if(xc2<0)xc2=0;
            yc2=(array[4]/(array[4]+array[2]))*174-27;  if(yc2>127)yc2=127;if(yc2<0)yc2=0;
            zxcorrection=(xc2-64)*4.0; if(zxcorrection<0)zxcorrection= -zxcorrection;
            zycorrection=(yc2-64)*8.0; if(zycorrection<0)zycorrection= -zycorrection;
            zc2+=zxcorrection+zycorrection;
        }

        xt1=(int)xc1;                                // stick1 xyz positions
        yt1=(int)yc1;
        zt1=(int)zc1;

        xt2=(int)xc2;                                // stick2 xyz positions
        yt2=(int)yc2;
        zt2=(int)zc2;

        if((zc1<resetlevel) && (triggerreset1==1)){triggerreset1=0;}   //stick1 trigger variables
        if((zc1>triggerlevel) && (triggerreset1==0))
        {
            triggerreset1=1;
            wh1=(int)(zc1-zc1p);
            xw1=xt1;
            yw1=yt1;
            whack1=1;
            trigtime1=t_time;
        }

        if((printbatonflag==1)  && (t_time>nextprintbatontime))
        {
            nextprintbatontime=t_time+1000;
            //for(iii=0;iii<8;iii++){printf("%6.0f", array[iii]);}
            ui->lvCondDisplay->addItem("x1=" + QString::number(xt1) + " y1=" + QString::number(yt1) + " z1=" + QString::number(zt1));
            ui->lvCondDisplay->addItem("x2=" + QString::number(xt2) + " y2=" + QString::number(yt2) + " z2=" + QString::number(zt2));
            ui->lvCondDisplay->addItem("xw1=" + QString::number(xw1) + " yw1=" + QString::number(yw1) + " wk=" + QString::number(wh1));
            ui->lvCondDisplay->addItem("");
            ui->lvCondDisplay->scrollToBottom();
        }
        if((printADflag==1)&&(t_time>nextprintbatontime))
        {
            nextprintbatontime=t_time+1000;
            for(ii=0;ii<8;ii++)
                ui->lvCondDisplay->addItem(QString::number(intarray[ii]));
            ui->lvCondDisplay->addItem("buttons = " + QString::number(buttons) + "   feet = " + QString::number(feet));
            ui->lvCondDisplay->addItem("");
            ui->lvCondDisplay->scrollToBottom();
        }
    }//if(arrayindex==8
}

void MainWindow::initializecond(void)
{
    int ii;

    printbuttonsforscore();
    sysexcase=midiip=midiop=midimessages=0;
    for(ii=0;ii<VOICES;ii++)
    {
        chordpi[ii] = chordp[ii] = CHORDSIZE * ii;
    }
    tempo=temposave=temposet=1;
    ctempo=vtemp=0;
    //tempo=temposave=1;temposet=ctempo=vtemp=0;
    whack1=whack2=b14minusdown=b14minusup=b15minusdown=b15minusup=0;
    scan=0;
    measure=1;
    if(startmeasure!=1) scan=1;
    endscoreflag=0;
    scoreop=scoreip=scoreadp=needscore=scorestate=scorechars=0;
    threshtime=nexttime=pt_time=pt_beat=controlchangetime=t_time;
    nexttime=0;
    readscorez();
    playmode=1;
    triggerFlag=1;

}

int MainWindow::keyvelfun(int v)
{
    int type,keyvel=0;
    type=dyntype[v];
    switch(type)
    {
        case X:
            if(dynamic[v]==1){keyvel=xt1;}
            else {keyvel=xt2;}
            break;
        case Y:
            if(dynamic[v]==1){keyvel=yt1;}
            else {keyvel=yt2;}
            break;
        case Z:
            if(dynamic[v]==1){keyvel=zt1;}
            else {keyvel=zt2;}
            break;
        case P:
            if(dynamic[v]<4){keyvel=buf[10+dynamic[v]]>>5;} //pots 1-3
            else{keyvel=buf[5];}                            //pot 4
            break;
        case V:
            keyvel=dynamic[v];
            break;
    }
    return keyvel;
}

void MainWindow::xformandsend(void)
{
    float xformvalue;
    if(xform[contp]==0)
    {
        midisend(CONTROLCHANGE,contch[contp],contno[contp],value);
    }
    else
    {
        xformvalue=xformb[contp]+(xforma[contp]*(float)value);
        value=(int)xformvalue;if(value<0)value=0;if(value>127)value=127;
        midisend(CONTROLCHANGE,contch[contp],contno[contp],value);
    }
}

void MainWindow::pcc(void)
{
    ui->lvCondDisplay->addItem("type NEXT COMMAND  (type Q to quit)");
    ui->lvCondDisplay->scrollToBottom();
}

void MainWindow::loadplay(void)
{
    int ii;
    for(ii=0;ii<16;ii++)
    {
        midisend(CONTROLCHANGE,ii,64,0);
    }

    nexttime=t_time;
    while(t_time<nexttime);

    char fileToOpen[256] = "";
    if(fileName != NULL)
    {
        QByteArray ba = fileName.toLatin1();
        strcpy(fileToOpen, ba);
        strcat(fileToOpen, ".p");
    }

    if((fp2=fopen(fileToOpen,"r+b")) != NULL)
    {
        initializecond();
        ui->lvCondDisplay->addItem((QString)fileToOpen);
        ui->lvCondDisplay->addItem("Conductor file opened successfully");
        ui->lvCondDisplay->scrollToBottom();
    }
    else
    {
        ui->lvCondDisplay->addItem("FILE NOT FOUND"); scoresok=0;
        ui->lvCondDisplay->scrollToBottom();
    }

    if(calibrationflag==0)
    {
        if(zt1>2000)
        {
            triggerlevel=.9*(float)zt1;resetlevel=.9*triggerlevel;sustainlevel=.5*triggerlevel;
            ui->lvCondDisplay->addItem("trigger level = " + QString::number(triggerlevel));
            calibrationflag=1;
            ui->lvCondDisplay->addItem("AUTOMATIC CALIBRATION trigger flag lvel = " + QString::number(triggerlevel));
            ui->lvCondDisplay->scrollToBottom();
        }
        else
        {
            triggerlevel=DEFAULTTRIGGERLEVEL;resetlevel=.9*triggerlevel;sustainlevel=.5*triggerlevel;
            ui->lvCondDisplay->addItem("*******************AUTOMATIC TRIGGER LEVEL CALIBRATION FAILED");
            ui->lvCondDisplay->addItem("*******************HOLD BATONS IN CENTER OF ANTENNA & PRESS BUTTON 1 AGAIN");
            ui->lvCondDisplay->scrollToBottom();
        }
    }//end if(calibrationflag==0
}

void MainWindow::playrecord(void)
{
    int i,c,v,keyno,keyvel,beg,end,accent,keyvela,tp;
    //gettimeofday(&tb,NULL);t_time=(((tb.tv_sec-starttime)*4000)+(tb.tv_usec/250));   //update conductor program clock
    c=scorkd[0];

    switch(c)
    {
        case NOTE:                             /* turns on a note or a chord */
            //printf("note= %d %d %d %d %d",scorkd[0],scorkd[1],scorkd[2],scorkd[3],scorkd[4]);
            v=scorkd[2];c=channel[v];beg=chordpi[v];end=chordp[v];
            keyvel=keyvelfun(v);
            for(i=beg;i<end;i++)midisend(NOTEON,c,chord[i],0);/*turns off notes that are
                                                            playing */
            i=3;
            while(i<scorkdend){                  /* turns on new note or notes */
                keyno=scorkd[i++];
                keyvela=scorkd[i++]+keyvel;
                if(keyvela<1)keyvela=1;if(keyvela>127)keyvela=127;
                //printf("%d %d %d\n",c,keyno,keyvela);
                midisend(NOTEON,c,keyno,keyvela);chord[beg++]=keyno;
            }

            chordp[v]=beg;
            break;
        case TRIG:                        /* record produced by a "/" in score */
            break;
        case REST:              /*turns off all notes playing in a given voice */
            v=scorkd[2];c=channel[v];beg=chordpi[v];end=chordp[v];
            for(i=beg;i<end;i++)midisend(NOTEON,c,chord[i],0);
            chordp[v]=beg;
            break;

        keyvel=keyvelfun(v);

        case MNOTEON:      /*turns on a single note. note must be ended with a
                           subsequent MNOTEOFF record*/
            v=scorkd[2];c=channel[v];keyno=scorkd[3];accent=scorkd[4];
            keyvel=keyvelfun(v);
            keyvela=accent+keyvel;
            if(keyvela<1)keyvela=1;if(keyvela>127)keyvela=127;
            midisend(NOTEON,c,keyno,keyvela);
            break;
        case MNOTEOFF:                              /* turns off a single note */
            v=scorkd[2];c=channel[v];keyno=scorkd[3];accent=scorkd[4];
            keyvel=keyvelfun(v);
            keyvela=accent+keyvel;
            if(keyvela<1)keyvela=1;if(keyvela>127)keyvela=127;
            midisend(NOTEON,c,keyno,0);
            break;
        case MEASURE:                         //print measure number, stop scan
            measure++;
            ui->lvCondDisplay->addItem("measure = " + QString::number(measure));
            ui->lvCondDisplay->scrollToBottom();
            if(measure>startmeasure)scan=0;
            break;
        case PITCHBEND:
            v=scorkd[2];c=channel[v]+1;
            midisend(PITCHWHEEL,c,scorkd[3],scorkd[4]);break;
        case ENDSCORE: triggerFlag=0;break;
        case PEDAL:              // "p" in score turns sustain pedal on
            v=scorkd[2];c=channel[v];midisend(CONTROLCHANGE,c-1,64,127);break;
        case UPPEDAL:            // "P" in score turns sustain pedal off
            v=scorkd[2];c=channel[v];midisend(CONTROLCHANGE,c-1,64,0);break;
        case CONTTEMPO:         // "w"--continuous tempo control with y1, ignor beats
            if(ctempo)
            {
                vtemp=0;
                ui->lvCondDisplay->addItem("END CONTINUOUS TEMPO");
                ui->lvCondDisplay->scrollToBottom();
            }
            else
            {
                vtemp=1;
                ui->lvCondDisplay->addItem("START CONTINUOUS TEMPO");
                ui->lvCondDisplay->scrollToBottom();
            }
            break;
        case FTEMP:                   /* "W"--fixed tempo control, ignor beats */
            if(ctempo)
            {
                ctempo=0;
                ui->lvCondDisplay->addItem("END FIXED TEMPO");
                ui->lvCondDisplay->scrollToBottom();
            }
            else
            {
                ctempo=1;
                ui->lvCondDisplay->addItem("START FIXED TEMPO");
                ui->lvCondDisplay->scrollToBottom();
            }
            break;
        case PROGRAMCHANGE:    // "t" in score will cause a midi program change
            v=scorkd[2];c=channel[v];midisendprogramchange(c,scorkd[3]);
            break;
        case CHANNELPROGRAMCHANGE:    //channel program change with bank select
            // &cctpp msb lsb channel=cc(1-16)  program=pp(0-127)  msb=bank select most significant byte
            //                                                     lsb=bank select least signifcant byte
            //example  &16t6  81 3 would select a harpsichord in a roland jv-1010 synthesizer
            ui->lvCondDisplay->addItem("channel program change");
            ui->lvCondDisplay->scrollToBottom();
            midisend(CONTROLCHANGE,scorkd[2]-1,0,scorkd[4]);
            midisend(CONTROLCHANGE,scorkd[2]-1,32,scorkd[5]);
            midisendprogramchange(scorkd[2]-1,scorkd[3]);
            break;
        case SETCHANNEL:// the midi channel for a given voice is kept in
            //        channel[]. it is set with an "h" in the score
            channel[scorkd[2]]=scorkd[3];
            break;
        case TEMPO:         // "v"--set a fixed tempo starting at next event
           if(modee==2){tempo=temposave=(float)((scorkd[2]+(scorkd[3]<<6)))*0.1;temposet=1;}
           else        {tempo=temposave=(float)((scorkd[2]+(scorkd[3]<<6)));temposet=1;}
           //printf("tempo= %f \n",tempo);
           break;
        case KEYVELOCITY:                  // "k"--setup keyvelocity control
            dynamic[scorkd[2]]=scorkd[3];dyntype[scorkd[2]]=scorkd[4];
            break;
        case BATON2:    // T in score turna baton 2 on or off as a source of beats
            if(baton2){baton2=0;}else{baton2=1;}
            break;
        case MARK:                   //rehersal mark--displayed during performance
            mark=scorkd[3];break;
        case DOTEQUALS:                        // change ratio of dot to comma
            tempo=(float)(((int)tempo*scorkd[3])/scorkd[4]);
            dotchange=1;dotro=scorkd[3];dotrn=scorkd[4];
            break;
        case CONTCONTROL:                 // setup patchcord for continuous control
            if(scorkd[6]==V)       // if control change is a constant, send only once
            {
                tp=scorkd[7]-scorkd[8];if(tp<0)tp=0;midisend(CONTROLCHANGE,scorkd[5],scorkd[4],tp);
            }
            else{           // if control change is baton position, setup patchchord
                contpatch=scorkd[3];contno[contpatch]=scorkd[4];
                contch[contpatch]=scorkd[5];controlerfn[contpatch]=scorkd[6];
                controlerfnv[contpatch]=scorkd[7];
                if(scorkd[8]==scorkd[9]){xform[contpatch]=0;}
                else{
                    tx0=(float)scorkd[8];tx1=(float)scorkd[9];ty0=(float)scorkd[10];ty1=(float)scorkd[11];
                    xform[contpatch]=1;
                    xforma[contpatch]= (ty1-ty0)/(tx1-tx0);
                    xformb[contpatch]= ((tx1*ty0)-(tx0*ty1))/(tx1-tx0);
                    //printf("xform,contpatch= %d %d \n",xform[contpatch],contpatch);
                }
            }
            break;
        case MODE:
            modee=scorkd[2];
            if(modee==1)
            {
                ui->lvCondDisplay->addItem("RUNS");
                ui->lvCondDisplay->addItem("x1->pitch  y1->loudness");
                ui->lvCondDisplay->addItem("x2->tempo  y2->jump interval");
                ui->lvCondDisplay->addItem("foot8->increase timbre number");
                ui->lvCondDisplay->addItem("foot4->decrease timbre number");
                ui->lvCondDisplay->scrollToBottom();
            }
            if(modee==2) ui->lvCondDisplay->addItem("RHYTHMICON");
            ui->lvCondDisplay->scrollToBottom();
            break;
        case INIT:                                                //initialization
            for(i=1;i<17;i++)channel[i]=i-1;               //default  channel#=voice#
            tempo=temposave=temposet=1;ctempo=vtemp=0;
            ui->lvCondDisplay->addItem("INIT");
            ui->lvCondDisplay->scrollToBottom();
            break;
        case NOACTION:
            break;
    }// end switch(c)
}

void MainWindow::readscorez(void)
{
    int iii,opcode,nextc;
    if(fp2==NULL || !playmode){goto endreadscorez;}
    nextbeat=0;
nextrecord:
    nextc=getc(fp2);if(nextc==EOF){playmode=0;charcount=0;goto endreadscorez;}else{charcount=nextc;}
    nextc=getc(fp2);if(nextc==EOF){playmode=0;opcode=NOACTION;goto endreadscorez;}else{opcode=nextc;}
    nextc=getc(fp2);if(nextc==EOF){playmode=0;nextbeat=0;goto endreadscorez;}else{nextbeat+=nextc;}
    if(opcode==NOOP){goto nextrecord;}
    scorkd[0]=opcode;
    scorkd[1]=nextbeat;
    scorkdend=charcount-1;
    if(charcount>3)
    {
        for(iii=2;iii<scorkdend;iii++)
        {
            nextc=getc(fp2);if(nextc==EOF){playmode=0;scorkd[iii]=0;goto endreadscorez;}else{scorkd[iii]=nextc;}
        }
    }
    scorkd[scorkdend]=0;
    beatsum+=nextbeat;
endreadscorez:;

}

void MainWindow::stkcontrol(void)
{
    int i,j;
    value=-1;
    i=controlerfn[contp];
    j=controlerfnv[contp];
    switch(i)
    {
        case X:if(j==1 ){value=xt1;xformandsend();}
            if(j==2 ){value=xt2;xformandsend();}
            break;
        case Y:if(j==1 ){value=yt1;xformandsend();}
            if(j==2 ){value=yt2;xformandsend();}
            break;
        case Z:if(j==1){value=zt1;xformandsend();}
            if(j==2){value=zt2;xformandsend();}
            break;
        case P:
            switch(j)
            {
                case 1:midisend(CONTROLCHANGE,contch[contp],contno[contp],pt1);break;
                case 2:midisend(CONTROLCHANGE,contch[contp],contno[contp],pt2);break;
                case 3:midisend(CONTROLCHANGE,contch[contp],contno[contp],pt3);break;
                case 4:midisend(CONTROLCHANGE,contch[contp],contno[contp],pt4);break;
            }
            break;
        case S:
            if(j==15){                                     //buf15- trigger
                if(!sustped[contch[contp]] && b15minusdown){       //down trig
                    b15minusdown=0;
                    sustped[contch[contp]]=1;midisend(CONTROLCHANGE,contch[contp],contno[contp],127);
                }
                if(sustped[contch[contp]] && b15minusup){          //up trig
                    b15minusup=0;
                    sustped[contch[contp]]=0;midisend(CONTROLCHANGE,contch[contp],contno[contp],0);
                }
            }
            break;
    }
}

void MainWindow::playscorefile(void)
{
    if(scan)
    {
        if(scorkd[0]==NOTE)goto readsk;
        if(scorkd[0]==MNOTEON)goto readsk;           //skip midi note on's
        if(scorkd[0]==MNOTEOFF)goto readsk;           //skip midi note on's
        if(scorkd[0]==REST)goto readsk;           //skip midi note on's
        playrecord();                                   //play score record
readsk:
        if(playmode)readscorez();               //read next score record
    }
    else  //0
    {
        if(t_time>controlchangetime)
        {
            controlchangetime=t_time+20;
            if(contno[contp])stkcontrol();         //send data from next patchchord contno[j] is zero for patchchord which is not in use
            if(++contp==NOCONTROL)contp=0;
        }
        if(nobat)                                             // in nobaton mode, play score at constant tempo (=tempo)
        {
            if(t_time>nexttime)
            {
                playrecord();if(playmode)readscorez();nexttime=t_time+(nextbeat*(int)tempo);
                // if(nextbeat>0)printf("tempo,t_time,nexttime,nextbeat= %f %d %d %d \n",tempo,t_time,nexttime,nextbeat);
            }
        }
        else //1
        {
            if(ctempo)                     // continuous tempo control by y1
            {
                if(t_time>nexttime)
                {
                    playrecord();if(playmode)readscorez();
                    if((zt1>set1) && (vtemp==1)){tempo=(temposave*64.0)/(yc1+20.0);}
                    nexttime=t_time+(nextbeat*(int)tempo);
                }
            }
            else //2
            {
                if(whack1)
                {
                    whack1=0;
                    if((t_time-lastwhacktime)<600) goto falsewhack;   //baton 1 control of tempo
                    lastwhacktime=t_time;
                    beat_time=trigtime1;itim=trigtime1-pt_time;
                    while(scorkd[0]!=TRIG)
                    {
                        if(scorkd[0]==NOTE)scorkd[0]=REST;  //skip unplayed notes
                        playrecord();if(playmode)readscorez();
                    }
                    ibeat=beatsum-pt_beat;pt_beat=beatsum;pt_time=beat_time;     //compute new tempo from time between last two baton beats
                    if(dotchange){ibeat=(ibeat*dotrn)/dotro;dotchange=0;}
                    if(!temposet){tempo=temposave=((float)itim/(float)ibeat);}          //printf("tempo,temposet= %f %d \n",tempo,temposet);}
                    else{temposet=0;}
                    if(playmode)readscorez();
                    nexttime=t_time+(int)(nextbeat*tempo);
falsewhack:;
                }//end if(whack1
                else //3
                {
                    if(scorkd[0]!=TRIG)
                    {
                        if(t_time>nexttime){playrecord();if(playmode)readscorez();nexttime=t_time+(int)((float)nextbeat*tempo);}
                    }
                }//end else 3
            }//end else 2
        }//end else 1
    }//end else 0
}

void MainWindow::playruns(void)
{
    if(play==1 && zc1<sustainlevel){ play=0;midisend(NOTEON,0,keyno,0);}  //note_on(1,keyno,0);
    if(whack1 && zc1>resetlevel)
    {
        keyno=xt1/2+30;
        midisend(NOTEON,0,keyno,yt1);noteon=anynoteon=1;
        nextnotetime=t_time+dur;
        play=1;
        whack1=0;
    }
    if(zc2>sustainlevel*.3)
    {
        dur=500-(xt2*4);if(dur<50)dur=50;
        interval=yt2/8;
    }

    if(zc2>sustainlevel)
    {
        if(!sustain){sustain=1;midisend(CONTROLCHANGE,0,64,127);}
    }
    else{if(sustain){sustain=0;midisend(CONTROLCHANGE,0,64,0);}}

    if(play && t_time>nextnotetime)
    {
        nextkeyno=xt1/2+30;
        if(noteon){midisend(NOTEON,0,keyno,0);noteon=0;}
        if(nextkeyno<keyno)keyno -= interval;
        else keyno += interval;
        midisend(NOTEON,0,keyno,yt1);noteon=1;
        nextnotetime=t_time+dur;
    }
}

void MainWindow::processbuttoncommand(void)
{
    ui->lvCondDisplay->addItem("Button Command " + QString::number(buttoncommand));
    ui->lvCondDisplay->scrollToBottom();
    int ii;
    switch(buttoncommand)
    {
        case 24: if(anynoteon==1){for(ii=0;ii<16;ii++)midisend(0xb,ii,0x78,0);anynoteon=0;}break;   //silence synthesizer with buttons 4&5
        case 1: feet=0;feetcommand=0;timbrenumber=0;loadplay();break;
        case 2: exitflag=1;ui->lvCondDisplay->addItem("Exiting");ui->lvCondDisplay->scrollToBottom();ui->lvMessages->insertItem(0, "Conductor program terminated");break;


        //ADD A MESSAGE BOX HERE TO ASK FOR A STARTING MEASURE!!!
        case 4: ui->lvCondDisplay->addItem("startmeasure = ");
                closeMidi(); //Pm_Terminate();
                scanf("%d",&startmeasure);printf("%d\n",startmeasure);startmidi();loadplay();break;



        case 6:if(printbatonflag==0){printbatonflag=1;ui->lvCondDisplay->addItem("PRINTING ON");ui->lvCondDisplay->scrollToBottom();}else{printbatonflag=0;ui->lvCondDisplay->addItem("PRINTING OFF");ui->lvCondDisplay->scrollToBottom();}break;
        case 22:if(printADflag==0){printADflag=1;ui->lvCondDisplay->addItem("PRINTING ON");ui->lvCondDisplay->scrollToBottom();}else{printADflag=0;ui->lvCondDisplay->addItem("PRINTING OFF");ui->lvCondDisplay->scrollToBottom();}break;
        case 8:  triggerlevel /=1.10;resetlevel=.9*triggerlevel;sustainlevel=.5*triggerlevel;ui->lvCondDisplay->addItem("raise triggerlevel=" + QString::number(triggerlevel));ui->lvCondDisplay->scrollToBottom();break;
        case 16: triggerlevel *=1.10;resetlevel=.9*triggerlevel;sustainlevel=.5*triggerlevel;ui->lvCondDisplay->addItem("lower triggerlevel=" + QString::number(triggerlevel));ui->lvCondDisplay->scrollToBottom();break;
        case 12: if(nobat==0)nobat=1;else nobat=0; ui->lvCondDisplay->addItem("nobat=" + QString::number(nobat));ui->lvCondDisplay->scrollToBottom();break;
        default: ui->lvCondDisplay->addItem("unknown button command=" + QString::number(buttoncommand));ui->lvCondDisplay->scrollToBottom();
    }
    buttoncommand=0;
}

void MainWindow::processfeetcommand(void)
{
    if(feetcommand==8)
    {
        timbrenumber++; if(timbre[timbrenumber]<0)timbrenumber=0;
        midisendprogramchange(0,timbre[timbrenumber]);
        printtimbre(timbre[timbrenumber]);
        feetcommand=0;
    }
    if(feetcommand==4)
    {
        timbrenumber--; if(timbrenumber<0)timbrenumber=0;
        midisendprogramchange(0,timbre[timbrenumber]);
        printtimbre(timbre[timbrenumber]);
        feetcommand=0;
    }
}

void MainWindow::printbuttonsforscore(void)
{
    ui->lvCondDisplay->addItem("BUTTON COMMANDS FOR SCORE");
    ui->lvCondDisplay->addItem("4&5-----SILENCE SYNTHESIZER");
    ui->lvCondDisplay->addItem("1-----load & play   set trigger levels");
    ui->lvCondDisplay->addItem("2-----exit conductor program");
    ui->lvCondDisplay->addItem("3-----set start measure");
    ui->lvCondDisplay->addItem("2&3---toggle xyz test print");
    ui->lvCondDisplay->addItem("3&4---toggle nobat mode");
    ui->lvCondDisplay->addItem("2&3&5-toggle 8-AD test print");
    ui->lvCondDisplay->addItem("4-----raise trigger level");
    ui->lvCondDisplay->addItem("5-----lower trigger level");
    ui->lvCondDisplay->addItem("");
    ui->lvCondDisplay->scrollToBottom();
}

void MainWindow::midisend(int status, int channel, int byte1, int byte2)
{
    bufferOut[0].timestamp = TIME_PROC(TIME_INFO);
    bufferOut[0].message = Pm_Message((unsigned char)((status<<4) | channel),(unsigned char)byte1,(unsigned char)byte2);
    Pm_Write(outStream, bufferOut, 1);

    if(status==9)anynoteon=1;
}

void MainWindow::midisendprogramchange(int channel, int byte1)
{
    bufferOut[0].timestamp = TIME_PROC(TIME_INFO);
    bufferOut[0].message = Pm_Message((unsigned char)(0xc0 | channel),(unsigned char)byte1,0);
    Pm_Write(outStream, bufferOut, 1);
}

void MainWindow::printtimbre(int timb)
{
    switch(timb)
    {
        case  0:ui->lvCondDisplay->addItem(QString::number(timbrenumber) + "piano");ui->lvCondDisplay->scrollToBottom();break;
        case  9:ui->lvCondDisplay->addItem(QString::number(timbrenumber) + "glockenspeil");ui->lvCondDisplay->scrollToBottom();break;
        case 11:ui->lvCondDisplay->addItem(QString::number(timbrenumber) + "vibraphone");ui->lvCondDisplay->scrollToBottom();break;
        case 14:ui->lvCondDisplay->addItem(QString::number(timbrenumber) + "tubular bells");ui->lvCondDisplay->scrollToBottom();break;
        case 46:ui->lvCondDisplay->addItem(QString::number(timbrenumber) + "harp");ui->lvCondDisplay->scrollToBottom();break;
        case 47:ui->lvCondDisplay->addItem(QString::number(timbrenumber) + "timpani");ui->lvCondDisplay->scrollToBottom();break;
        default:  ui->lvCondDisplay->addItem(QString::number(timbrenumber) + "unknown timbre");ui->lvCondDisplay->scrollToBottom();break;
    }
}

void MainWindow::playrhythmicon(void)
{
    if(scan)//0
    {
        if(scorkd[0]==NOTE)goto readsk;
        if(scorkd[0]==MNOTEON)goto readsk;          //skip midi note on's
        if(scorkd[0]==MNOTEOFF)goto readsk;         //skip midi note on's
        if(scorkd[0]==REST)goto readsk;             //skip midi note on's
        playrecord();                               //play score record
readsk:
        if(playmode)readscorez();                   //read next score record
    }
    else//0
    {
        if(t_time>controlchangetime)
        {
            controlchangetime=t_time+20;

            if(contno[contp])stkcontrol(); //send data from next patchchord contno[j]=0 for unused patchchord
            if(++contp==NOCONTROL)contp=0;
            if(vtemp==1){tempo=(temposave*64)/(xc2+20);}
        }
        if(nobat)//1                                // in nobaton mode, play score at constant tempo (=tempo)
        {
            if(t_time>nexttime)
            {
                playrecord();if(playmode)readscorez();nexttime=t_time+(nextbeat*(int)tempo);
                // if(nextbeat>0)printf("tempo,t_time,nexttime,nextbeat= %f %d %d %d \n",tempo,t_time,nexttime,nextbeat);
            }
        }
        else//1
        {
            if(ctempo)     //2                // continuous tempo control by y1
            {
                if(t_time>nexttime)
                {
                    playrecord();if(playmode)readscorez();
                    if(zt1>set1 && vtemp){tempo=(temposave*64)/(xc2+20);}
                    nexttime=t_time+(nextbeat*(int)tempo);
                }
            }
            else//2
            {
                if(whack1)//3
                {
                    whack1=0;
                    if((t_time-lastwhacktime)<600) goto falsewhack;   //baton 1 control of tempo
                    lastwhacktime=t_time;
                    beat_time=trigtime1;itim=trigtime1-pt_time;
                    while(scorkd[0]!=TRIG)
                    {
                        if(scorkd[0]==NOTE)scorkd[0]=REST;  //skip unplayed notes
                        playrecord();if(playmode)readscorez();
                    }
                    if(vtemp==1){tempo=(temposave*64)/(xc2+20);}
                    else
                    {
                        ibeat=beatsum-pt_beat;pt_beat=beatsum;pt_time=beat_time;     //compute new tempo from time between last two baton beats
                        if(dotchange){ibeat=(ibeat*dotrn)/dotro;dotchange=0;}
                        if(!temposet){tempo=temposave=((float)itim/(float)ibeat);} else{temposet=0;}
                    }
                    if(playmode)readscorez();
                    nexttime=t_time+(int)(nextbeat*tempo);
falsewhack:;
                }
                else //3
                {
                    if(scorkd[0]!=TRIG) {if(t_time>nexttime) {playrecord();if(playmode)readscorez();nexttime=t_time+(int)((float)nextbeat*tempo); } }
                }//end else 3
            }//end else 2
        }//end else 1
    }//end else 0
}

int MainWindow::get_number(char *prompt)
{
    char line[STRING_MAX];
    int n = 0, i;


    //NEED TO ADD A PROMPT MESSAGE BOX HERE!


    printf(prompt);
    while (n != 1) {
        n = scanf("%d", &i);
        fgets(line, STRING_MAX, stdin);
    }
    return i;
}

void MainWindow::startmidi()
{
    TIME_START;
    openMidi();

    //clear buffers
    while (Pm_Poll(inStream))
    {
        Pm_Read(inStream, bufferIn, 1);
    }
}

void MainWindow::setdevices()
{
    deviceIn = inDev;
    deviceOut = outDev;
}

void MainWindow::thru()
{
    if(ui->cbThru->checkState())
    {
        mThru = 1;
        ui->lvMessages->insertItem(0, tr("MIDI thru is now on."));
    }
    else
    {
        mThru = 0;
        ui->lvMessages->insertItem(0, tr("MIDI thru is now off."));
    }
}

void MainWindow::showEditWindow()
{
    EditDialog* edit = new EditDialog(this, &fileName);
    edit->exec();
    delete edit;

    //chop off all the directories from the file name and print out only the blah.txt
    QStringList list;
    list = fileName.split("/");
    ui->lConductFile->setText(list.last());

    if(!fileName.isEmpty())
        ui->lvMessages->insertItem(0, tr("Finished editing ") + list.last() + tr(": Don't forget to compile."));
}

void MainWindow::updateDevices()
{
    int i, cnt;
    totalInDevices = 0;
    const PmDeviceInfo *info;

    updating = 1;

    ui->cbMidiIn->clear();
    ui->cbMidiOut->clear();

    if(mTimerOn) stopMidiTimer();
    closeMidi();
    Pm_Terminate();
    Pm_Initialize();

    if((cnt = Pm_CountDevices()))	//return the number of devices
    {
        for(i=0; i < cnt; i++)
        {
            info = Pm_GetDeviceInfo(i); //gets device info
            if(info->input)  //if input
            {
                ui->cbMidiIn->addItem(info->name, NULL);
                totalInDevices++;
                if(!inOpen)
                {
                    inDev = i;
                    openMidi();
                }
            }
            if(info->output)
            {
                ui->cbMidiOut->addItem(info->name, NULL);
                if(!outOpen)
                {
                    outDev = i;
                    openMidi();
                }
            }
        }
    }

    updating = 0;

    changeFilter();
}

void MainWindow::compile()
{
    char sysMessage[1000];

    QString ba1 = fileName;
    ba1.replace(QString(" "), QString("\\ "));
    QByteArray ba = ba1.toLatin1();

    QDir dir;
    dir = dir.currentPath();
    dir.cdUp();
    dir.cdUp();
    dir.cdUp();
    QByteArray cd = dir.path().toLatin1();

    QString cd1 = cd;
    cd1.replace(QString(" "), QString("\\ "));

    QByteArray curDir = cd1.toLatin1();

    strcpy(sysMessage, (char*)curDir.data());
    strcat(sysMessage, "/compile ");
    strcat(sysMessage, (char*)ba.data());
    strcat(sysMessage, " ");
    strcat(sysMessage, (char*)ba.data());
    strcat(sysMessage, ".p");

    if(!system(sysMessage))
        ui->lvMessages->insertItem(0, "File compiled successfully!");
    else
        ui->lvMessages->insertItem(0, "There was an error compiling!");
}

void MainWindow::openFile()
{
    if(mTimerOn) stopMidiTimer();
    closeMidi();

    //Open dialog box - the file name is stored in the QString fileName for modification later
    fileName = QFileDialog::getOpenFileName(this,
         tr("Open Conductor File"));

    openMidi();

    //chop off all the directories from the file name and print out only the blah.txt
    QStringList list;
    list = fileName.split("/");
    ui->lConductFile->setText(list.last());

    if(!fileName.isEmpty())
        ui->lvMessages->insertItem(0, tr("Opened ") + list.last() + tr(": Don't forget to compile."));
}

//Change the "conduct program" mode
void MainWindow::modeChange()
{
    if(mTimerOn) stopMidiTimer();
    closeMidi();
    modee = ui->cbMode->currentIndex();
    openMidi();
}

void MainWindow::changeFilter()
{
    /*long filt = NULL;
    Pm_SetFilter(inStream, PM_FILT_RESET);
    if(ccFilt) filt = filt | PM_FILT_CONTROL;
    if(noteFilt) filt = filt | PM_FILT_NOTE;
    if(polyFilt) filt = filt | PM_FILT_POLY_AFTERTOUCH;
    if(chanFilt) filt = filt | PM_FILT_CHANNEL_AFTERTOUCH;
    if(progFilt) filt = filt | PM_FILT_PROGRAM;
    if(pitchFilt) filt = filt | PM_FILT_PITCHBEND;
    if(sysFilt) filt = filt | PM_FILT_SYSEX;
    if(realFilt) filt = filt | PM_FILT_REALTIME | PM_FILT_SYSTEMCOMMON;
    PmError err;
    err = Pm_SetFilter(inStream, filt);*/

    Pm_SetFilter(inStream, PM_FILT_CONTROL);
}

void MainWindow::fControllerChange()
{
    if(ui->cbControllerChange->isChecked())
    {
        ui->lvMessages->insertItem(0, tr("Controller change messages now accepted"));
        ccFilt = 0;
        changeFilter();
    }
    else
    {
        ui->lvMessages->insertItem(0, tr("Controller change messages no longer accepted"));
        ccFilt = 1;
        changeFilter();
    }
}

void MainWindow::fSysEx()
{
    if(ui->cbSysEx->isChecked())
    {
        ui->lvMessages->insertItem(0, tr("SysEx messages now accepted"));
        sysFilt = 0;
        changeFilter();
    }
    else
    {
        ui->lvMessages->insertItem(0, tr("SysEx messages no longer accepted"));
        sysFilt = 1;
        changeFilter();
    }
}

void MainWindow::fRealTime()
{
    if(ui->cbRealTime->isChecked())
    {
        ui->lvMessages->insertItem(0, tr("Real-time messages now accepted"));
        realFilt = 0;
        changeFilter();
    }
    else
    {
        ui->lvMessages->insertItem(0, tr("Real-time messages no longer accepted"));
        realFilt = 1;
        changeFilter();
    }
}

void MainWindow::fMonoAftertouch()
{
    if(ui->cbMonoAftertouch->isChecked())
    {
        ui->lvMessages->insertItem(0, tr("Mono aftertouch messages now accepted"));
        chanFilt = 0;
        changeFilter();
    }
    else
    {
        ui->lvMessages->insertItem(0, tr("Mono aftertouch messages no longer accepted"));
        chanFilt = 1;
        changeFilter();
    }
}

void MainWindow::fNote()
{
    if(ui->cbNote->isChecked())
    {
        ui->lvMessages->insertItem(0, tr("Note messages now accepted"));
        noteFilt = 0;
        changeFilter();
    }
    else
    {
        ui->lvMessages->insertItem(0, tr("Note messages no longer accepted"));
        noteFilt = 1;
        changeFilter();
    }
}

void MainWindow::fPitchBend()
{
    if(ui->cbPitchBend->isChecked())
    {
        ui->lvMessages->insertItem(0, tr("Pitch bend messages now accepted"));
        pitchFilt = 0;
        changeFilter();
    }
    else
    {
        ui->lvMessages->insertItem(0, tr("Pitch bend messages no longer accepted"));
        pitchFilt = 1;
        changeFilter();
    }
}

void MainWindow::fPolyAftertouch()
{
    if(ui->cbPolyAftertouch->isChecked())
    {
        ui->lvMessages->insertItem(0, tr("Poly aftertouch messages now accepted"));
        polyFilt = 0;
        changeFilter();
    }
    else
    {
        ui->lvMessages->insertItem(0, tr("Poly aftertouch messages no longer accepted"));
        polyFilt = 1;
        changeFilter();
    }
}

void MainWindow::fProgramChange()
{
    if(ui->cbProgramChange->isChecked())
    {
        ui->lvMessages->insertItem(0, tr("Program change messages now accepted"));
        progFilt = 0;
        changeFilter();
    }
    else
    {
        ui->lvMessages->insertItem(0, tr("Program change messages no longer accepted"));
        progFilt = 1;
        changeFilter();
    }
}

MainWindow::~MainWindow()
{
    if(mTimerOn) stopMidiTimer();
    closeMidi();
    Pm_Terminate();

    delete ui;
}

void MainWindow::changeMidiIn()
{
    if(!updating)
    {
        const PmDeviceInfo *info;
        info = Pm_GetDeviceInfo(ui->cbMidiIn->currentIndex());

        if(mTimerOn) stopMidiTimer();
        closeMidi();
        inDev = ui->cbMidiIn->currentIndex();
        openMidi();

        ui->lvMessages->insertItem(0, tr("MIDI input device changed to ") + (QString)info->name);
        changeFilter();
    }
}

void MainWindow::changeMidiOut()
{
    if(!updating)
    {
        const PmDeviceInfo *info;
        info = Pm_GetDeviceInfo(ui->cbMidiOut->currentIndex() + totalInDevices);

        if(mTimerOn) stopMidiTimer();
        closeMidi();
        outDev = ui->cbMidiIn->currentIndex() + totalInDevices;
        openMidi();

        ui->lvMessages->insertItem(0, tr("MIDI output device changed to ") + (QString)info->name);
        changeFilter();
    }
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void MainWindow::checkAllFilters()
{
    ui->cbControllerChange->setCheckState(Qt::CheckState(Qt::Checked));
    ui->cbNote->setCheckState(Qt::CheckState(Qt::Checked));
    ui->cbMonoAftertouch->setCheckState(Qt::CheckState(Qt::Checked));
    ui->cbPitchBend->setCheckState(Qt::CheckState(Qt::Checked));
    ui->cbPolyAftertouch->setCheckState(Qt::CheckState(Qt::Checked));
    ui->cbProgramChange->setCheckState(Qt::CheckState(Qt::Checked));
    ui->cbSysEx->setCheckState(Qt::CheckState(Qt::Checked));
    ui->cbRealTime->setCheckState(Qt::CheckState(Qt::Checked));
}

void MainWindow::initMidi()
{
    int i, cnt;
    totalInDevices = 0;

    event = (PmEvent *)malloc(sizeof(PmEvent));

    const PmDeviceInfo *info;

    Pm_Initialize();	//Init PortMIDI

    if((cnt = Pm_CountDevices()))	//return the number of devices
    {
        for(i=0; i < cnt; i++)
        {
            info = Pm_GetDeviceInfo(i); //gets device info
            if(info->input)  //if input
            {
                ui->cbMidiIn->addItem(info->name, NULL);
                if(!inOpen)
                {
                    Pm_OpenInput(&inStream, i, NULL, 512, NULL, NULL);
                    ui->lvMessages->insertItem(0, tr("Opened the MIDI device ") + info->name + " for input.");
                    inDev = i;
                    inOpen = 1;
                }
                totalInDevices++;
            }
            if(info->output)
            {
                ui->cbMidiOut->addItem(info->name, NULL);
                if(!outOpen)
                {
                    Pm_OpenOutput(&outStream, i, NULL, 512, NULL, NULL, 0);
                    ui->lvMessages->insertItem(0, tr("Opened the MIDI device ") + info->name + " for output.");
                    outDev = i;
                    outOpen = 1;
                }
            }
        }
    }
}


void MainWindow::startMidiTimer()
{
    //Timer to read MIDI messages
    connect(mTimer, SIGNAL(timeout()), this, SLOT(updateMessages()));
    mTimer->start(1);
    mTimerOn = 1;
}

void MainWindow::stopMidiTimer()
{
    mTimer->disconnect();
    mTimerOn = 0;
}

void MainWindow::updateMessages()
{
    long status, data1, data2;

    if(!condClockOn && !matrixMode && inOpen)
    {
        if(Pm_Poll(inStream)) //was there an event?
        {
            Pm_Read(inStream, event, sizeof(long));
            if(ui->lvMessages->count() > 200) ui->lvMessages->clear();
            //translate the message to longs
            status = Pm_MessageStatus(event->message);
            data1 = Pm_MessageData1(event->message);
            data2 = Pm_MessageData2(event->message);

            if(dispMidi)
            {
                QString message = "MIDI Input Event: status: 0x" + QString::number(status, 16) + "  data byte 1: " + QString::number(data1) + "  data byte 2: " + QString::number(data2);
                ui->lvMessages->insertItem(0, message);
            }

            if(mThru)
            {
                Pm_Write(outStream, event, 1);
                if(dispMidi)
                {
                    QString message = "MIDI Output Event: status: 0x" + QString::number(status, 16) + "  data byte 1: " + QString::number(data1) + "  data byte 2: " + QString::number(data2);
                    ui->lvMessages->insertItem(0, message);
                }
            }
        }
    }
}
