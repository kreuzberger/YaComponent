.. YaComponent UserGuide documentation master file, created by
   sphinx-quickstart on Tue Dec  2 19:17:26 2014.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

Welcome to YaComponent UserGuide's documentation!
=================================================

.. uml::

  interface AudioPlayer
  package WavFileReader
  WavFileReader - AudioPlayer

  package SoundOut
  SoundOut  -left-> AudioPlayer


The code for the interface is a simple google protocol buffer file, save with extension proto e.g. to AudioPlayerIfc.proto::

  message AudioData {
    required double samplerate = 1;
    required int32 samplecount = 2;
    required int32 channels = 3;
    required bool complex = 4;
    required bool interleaved = 5;
  
    enum Format {
      INTEGER_SIGNED_16_BIT = 0;
      INTEGER_SIGNED_8_BIT = 1;
      FLOAT_LITTLE_ENDIAN = 2;
      DOUBLE_LITTLE_ENDIAN = 3;
    }
    
    required Format format = 6;
    required bytes data = 7;
  }


  message StartData {
  }
  
  message StopData {
  }
  
  message StoppedData {
  }
  
  message StartedData {
  }


For later reuse of the message type AudioData independent form the messages for request or response, it is usefull to save the message type AudioData in a seperate .proto file and import it in the AudioPlayerIfc.proto file.


The interace definition for the Interface is no quite simple defined in the xml file::

  <interface>
    <version maj="1" min="1" rev="0"/>
    <include file="AudioPlayerIfc.pb.h"/>
    <properties>
      <prop id="AudioData" package=""/>
    </properties>
    <requests>
      <req id="StartData" package="">
      <resp id="StartedData" package=""/>
      </req>
      <req id="StopData" package="">
      <resp id="StoppedData" package=""/>
      </req>
    </requests>
    <responses>
      <resp id="StartedData" package=""/>
      <resp id="StoppedData" package=""/>
    </responses>
  </interface>
  
The interace describes now the properties send of type AudioData, the request to start and stop data delivery and the repsonsed to the requests.


To define the components, we use another xml file.
First we try to define the data providing component WavFileReader. This is also done with an xml file, defining the components provided and used interfaces.
E.g. content of WavFilePlayerComponent.xml::

  <component>
    <version maj="1" min="1" rev="0"/>
    <description/>
    <provides>
      <interface id="AudioPlayer"   xml="AudioPlayerIfc.xml"/>
    </provides>
    <uses/>
  </component>
  
The provided interface is defined by giving it an id, from with the code implemenation takes the later attribute name for the interface. The interface definition itself is referened via the location of the xml file.

If the component does not use any other components, leave the uses seciton empty or omit.

To define the SoundOut component, we just add another xml file, defining the component and its used interface, e.g. in file SoundOutComponent.xml::

  <component>
    <version maj="1" min="1" rev="0"/>
    <description/>
    <provides/>
    <uses>
      <interface id="AudioPlayer" xml="AudioPlayerIfc.xml"/>
    </uses>
  </component>
  


But how are the components linked together? This is a question of your application and process layout. Do you want to have both components in one process/application? Or do you want to seperate them?
In our example we decide to intergrate them into a single application and generate a layout descpriont xml file, e.g. called WavPlayer.xml::

  <componentlayout>
  <processes>
  <process name="WavPlayer">
    <description></description>
    <thread name="WavPlayerThread">
      <component name="WavFilePlayer" xml="WavFilePlayerComponent.xml">
        <provides>
          <interface id="AudioPlayer" address="inproc:///tmp/audioprovider" hwm="5000"/>
        </provides>
      </component>
    </thread>
    <component name="SoundOut" xml="SoundOutComponent.xml">
      <uses>
        <interface id="AudioPlayer" sub="WavFilePlayer.AudioPlayer"/>
      </uses>
    </component>
  </process>
  </processes>
  </componentlayout>
  

The component overview gives you and hint how the components are now connected:

.. uml::

  package WavPlayer {
    package WavPlayerThread {
      frame WavFileReader
      WavFileReader - AudioPlayer :   inproc://tmp/audioprovider
    }
  
    package main {
      frame SoundOut
      SoundOut  -left-> AudioPlayer
    }
  }
  

After all definitions, the code generation parts now dow their jobs. To start code generation, you must use the perl scripts provided with the project or use the predefined cmake makros to generate your code from the xml file and proto files.

A snipped from a CMakeLists.txt used to build the examples files contains the macro calls::

  PROTOBUF_GENERATE_CPP(ProtoSourcesAudioPlayer ProtoHeadersAudioPlayer AudioPlayerIfc.proto)
  
  YACOMPONENT_IFC_GENERATE(GENERATED_IFC_AUDIOPLAYER AudioPlayerIfc.xml )
  
  
  YACOMPONENT_GENERATE(GENERATED_COMP_WAV_SRC WavFilePlayerComponent.xml)
  YACOMPONENT_GENERATE(GENERATED_COMP_SOUND_SRC SoundOutComponent.xml)
  
  YACOMPONENT_LAYOUT_GENERATE(GENERATED_LAYOUT WavPlayer.xml)
  
The generated code from the layout could then be used in your main application::

  void* context = YaComponent::context_new();
  YaComponentThread WavPlayerThread;
  WavPlayerThread.start();
  WavFilePlayerComponent WavFilePlayer(context);
  WavFilePlayer.setConnectionParaAudioPlayer( "inproc:///tmp/audioprovider", 5000 );
  WavFilePlayer.init();
  WavFilePlayer.moveToThread(&WavPlayerThread);
  SoundOutComponent SoundOut(context);
  SoundOut.setConnectionParaAudioPlayer( "inproc:///tmp/audioprovider", "SoundOut" );
  SoundOut.init();  
  

As you can see, the two concrete classes WavFilePlayerComponent and SoundOutComponent must be used. This is now the developers part to create this classes and define their behaviour. There are classes derived from the generated classes WavFileReaderComponentImpl and SoundOutComponentImpl with handlers for the pure virtual functions to handle properties, requests and responses.

.. uml::

  class WavFilePlayerComponent -up--|> WavFilePlayerComponentImpl
  
  class SoundOutComponent -up--|> SoundOutComponentImpl
  
  class WavFilePlayerComponent {
    onRequestStartData( int id , StartedData& ) 
    onRequestStopData( int id , StoppedData& ) 
  }
  
  
  class SoundOutComponent {
    virtual void onProperty( int id, const AudioData& );
    virtual void onResponse( int proxyId, const StartedData& );
    virtual void onResponse( int proxyId, const StoppedData& );
  }


For the SoundOutComponent theres now an active part to implmement

 * set notifications for AudioData properties
 * start data by calling StartData request
 * wait for end of data or call StopData request
 
Its also up to the developer to decide which file is played. This could be part of an additional method in WavFilePlayer to set the filename e.g. from commandline arguments.

The Id's provided in the generated interfaces are useful if there is a multiple use of the same interface or same message types in different interfaces. So you can handle the request properly depending on the interface you received them. In our example, the id's could be ignored.


Contents:

.. toctree::
   :maxdepth: 2



Indices and tables
==================

* :ref:`genindex`
* :ref:`modindex`
* :ref:`search`

