#include "textoutcompunittest.h"
#include "TextGenIfcProxy.h"

TextOutComp::TextOutComp(void *context)
    : YaComponent::TextOutCompImpl(context, static_cast<ITextGenIfcProxyHandler &>(*this))
    , ITextGenIfcProxyHandler(*this)
    , miPropertiesCnt(0)
    , miResponseStartCnt(0)
    , miResponseStopCnt(0)
    , miResponseTerminatedCnt(0)
{
    //  setNotification(YaComponent::TextGenIfcProxy::PROP_TEXTGEN_TEXT);
}

void TextOutComp::init()
{
    TextOutCompImpl::init();
}

void TextOutComp::setNotifications()
{
    mXml.setNotification(YaComponent::TextGenIfcProxy::PROP_TEXTGEN_TEXT);
    //  mPlain.setNotification(YaComponent::TextGenIfcProxy::PROP_TEXTGEN_TEXT);
}

void TextOutComp::requestStart()
{
    TextGen::Request oRequest;
    oRequest.set_id(1);
    // int size = oRequest.ByteSizeLong();
    mXml.requestStartText(oRequest);
}

void TextOutComp::requestStop()
{
    TextGen::Request oRequest;
    oRequest.set_id(2);
    mXml.requestStopText(oRequest);
}

void TextOutComp::requestTerminate()
{
    mXml.requestTerminate();
}

void TextOutComp::onPropertyText(int proxyId, const TextGen::Text & /*text*/)
{
    //  fprintf(stderr, "received property text %s\n",
    //  text.DebugString().c_str());
    assert(PROXY_XML == proxyId);
    miPropertiesCnt++;
}

void TextOutComp::onResponseStartedText( int proxyId, const TextGen::RequestInfo& resp )
{
    assert(PROXY_XML == proxyId);
    fprintf(stderr, "received onResponse startedText\n");
    assert(1 == resp.id());
    miResponseStartCnt++;
}

void TextOutComp::onResponseStoppedText( int proxyId, const TextGen::RequestInfo& resp )
{
    fprintf(stderr, "received onResponse stoppedText\n");
    assert(PROXY_XML == proxyId);
    assert(2 == resp.id() || 4242 == resp.id());
    miResponseStopCnt++;
}

void TextOutComp::onResponseTerminated(int proxyId)
{
    fprintf(stderr, "received onResponseTerminated\n");
    assert(PROXY_XML == proxyId);
    miResponseTerminatedCnt++;
}
