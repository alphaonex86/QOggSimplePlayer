#ifndef QOGGSIMPLEPLAYER_H
#define QOGGSIMPLEPLAYER_H

#include <QObject>

#include <QAudioOutput>
#include <QBuffer>

#ifdef _WIN32
#include <io.h>
#include <fcntl.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vorbis/codec.h>
#include <vorbis/vorbisfile.h>

#include "QOggAudioBuffer.h"

class QOggSimplePlayer : public QObject
{
    Q_OBJECT
public:
    explicit QOggSimplePlayer(const QString &filePath, QObject *parent = 0);
    virtual ~QOggSimplePlayer();
    void start();
    void stop();
private slots:
    void finishedPlaying(QAudio::State state);
    void readDone();
    void readMore();
private:
    QAudioOutput *output;
    QOggAudioBuffer buffer;
    FILE * file;
    OggVorbis_File vf;
    int current_section;
};

#endif // QOGGSIMPLEPLAYER_H
