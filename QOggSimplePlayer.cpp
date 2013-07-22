#include "QOggSimplePlayer.h"

#include <QDebug>

#define MAX_BUFFER_SIZE 64*1024
#define MIN_BUFFER_SIZE 16*1024

QOggSimplePlayer::QOggSimplePlayer(const QString &filePath,QObject *parent) :
    QObject(parent)
{
    output=NULL;
    buffer.open(QIODevice::ReadWrite|QIODevice::Unbuffered);
    QAudioFormat format;
    // Set up the format, eg.
    format.setFrequency(48000);
    format.setChannels(1);
    format.setSampleSize(16);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::SignedInt);

    file=fopen(filePath.toLocal8Bit().data(),"r");
    if(file==NULL)
    {
        qDebug() << "Unable to open the file";
        return;
    }

    if(ov_open_callbacks(/*stdin*/file, &vf, NULL, 0, OV_CALLBACKS_STREAMONLY) < 0)
    {
        qDebug() << "Input does not appear to be an Ogg bitstream";
        return;
    }

    {
        char **ptr=ov_comment(&vf,-1)->user_comments;
        vorbis_info *vi=ov_info(&vf,-1);
        while(*ptr){
            fprintf(stderr,"%s\n",*ptr);
            ++ptr;
        }
        qDebug() << QString("Bitstream is %1 channel, %2Hz").arg(vi->channels).arg(vi->rate);
        format.setChannels(vi->channels);
        format.setFrequency(vi->rate);
        format.setSampleRate(vi->rate);
        qDebug() << QString("Encoded by: %1").arg(ov_comment(&vf,-1)->vendor);
    }
    QAudioDeviceInfo info(QAudioDeviceInfo::defaultOutputDevice());
    if (!info.isFormatSupported(format)) {
        qDebug() << "raw audio format not supported by backend, cannot play audio.";
        return;
    }

    if(output!=NULL)
        delete output;
    output = new QAudioOutput(format);
    output->setBufferSize(4096);

    connect(&buffer,SIGNAL(readDone()),SLOT(readDone()));
    connect(output,SIGNAL(stateChanged(QAudio::State)),SLOT(finishedPlaying(QAudio::State)),Qt::QueuedConnection);
}

QOggSimplePlayer::~QOggSimplePlayer()
{
    buffer.close();
    ov_clear(&vf);
}

void QOggSimplePlayer::readMore()
{
    char pcmout[4096];
    bool eof=false;

    buffer.seek(buffer.data().size());
    while(!eof && buffer.data().size()<MAX_BUFFER_SIZE){
        long ret=ov_read(&vf,pcmout,sizeof(pcmout),0,2,1,&current_section);
        if (ret == 0) {
            /* EOF */
            eof=true;
        } else if (ret < 0) {
            /* error in the stream.  Not a problem, just reporting it in
            case we (the app) cares.  In this case, we don't. */
        } else {
            /* we don't bother dealing with sample rate changes, etc, but
            you'll have to */
            QByteArray tempData(pcmout,ret);
            //int pos=buffer.pos();
            buffer.write(tempData);
            //buffer.seek(pos);
        }
    }
    buffer.seek(0);
}

void QOggSimplePlayer::finishedPlaying(QAudio::State state)
{
    if (state == QAudio::IdleState)
        stop();
}

void QOggSimplePlayer::start()
{
    stop();
    readMore();
    output->start(&buffer);
}

void QOggSimplePlayer::stop()
{
    output->stop();
    buffer.seek(0);
    buffer.close();
    buffer.setData(QByteArray());
    buffer.open(QIODevice::ReadWrite|QIODevice::Unbuffered);
    current_section=0;
    ov_time_seek(&vf,0);
    fseek(file,0,SEEK_SET);
}

void QOggSimplePlayer::readDone()
{
    if(buffer.data().size()<MIN_BUFFER_SIZE)
        readMore();
}
