#include "textoutcomp.h"

TextOutComp::TextOutComp( void* context )
 : ITextGenIfcProxyHandler( self() )
, YaComponent::TextOutCompImpl( context, static_cast<ITextGenIfcProxyHandler&>(self()) )
{
//  setNotification(YaComponent::TextGenIfcProxy::PROP_TEXTGEN_TEXT);

}
