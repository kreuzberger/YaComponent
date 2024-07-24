#include "textgencompunittest.h"

TextGenComp::TextGenComp(void *context)
    : YaComponent::TextGenCompImpl(context, static_cast<ITextGenIfcStubHandler &>(*this))
    , ITextGenIfcStubHandler(*this)
    , miRequestStop(0)
    , miTimerID(-1)
    , miMessageCnt(0)
    , miMaxMessageCnt(1)
    , mbSendAutoStop(false)
    , moText()
{}

void TextGenComp::init()
{
    TextGenCompImpl::init();
}

void TextGenComp::onRequestStartText( int /*id*/, const TextGen::Request& oRequest, TextGen::RequestInfo& oResponse )
{
    fprintf(stderr, "received onRequestStartText, start sending text\n");
    oResponse.set_id(oRequest.id());
    miMessageCnt = 0;
    timerEvent(0);
    miTimerID = QObject::startTimer(20);
}

void TextGenComp::onRequestStopText( int /*id*/, const TextGen::Request& oRequest, TextGen::RequestInfo& oResponse )
{
    fprintf(stderr, "received onRequestStopText\n");
    oResponse.set_id(oRequest.id());
    miRequestStop++;
    if (0 < miTimerID) {
        QObject::killTimer(miTimerID);
    }
    fprintf(stderr, "send %d messages\n", miMessageCnt);
}

void TextGenComp::onRequestTerminate(int /*id*/)
{
    fprintf(stderr, "received onRequestTerminate\n");
}

void TextGenComp::sendResponseStop()
{
  TextGen::RequestInfo oResponse;
  oResponse.set_id( 4242 );
  mTextGenXml.responseStoppedText( YaComponent::TextGenIfcStub::RESP_TEXTGEN_STOPPEDTEXT, "", oResponse );
  if ( 0 < miTimerID )
  {
    QObject::killTimer( miTimerID );
    }
    fprintf(stderr, "send %d messages\n", miMessageCnt);
}

void TextGenComp::timerEvent(QTimerEvent *)
{
    QString oText("%1: The quick brown fox jumps over the lazy dog 0123456789\n");
    int rc = -1;

    do {
        moText.set_text(qPrintable(oText.arg(miMessageCnt)));
        rc = mTextGenXml.send(YaComponent::TextGenIfcStub::PROP_TEXTGEN_TEXT, moText);
        // rc = mTextGenPlain.send(YaComponent::TextGenIfcStub::PROP_TEXTGEN_TEXT, moText);
        if (-1 != rc) {
            miMessageCnt++;
        }
        if (miMessageCnt >= miMaxMessageCnt) {
            if (0 < miTimerID) {
                QObject::killTimer(miTimerID);
                miTimerID = 0;
            }
            if (mbSendAutoStop)
                sendResponseStop();
            break;
        }
    } while (rc != -1);
}
