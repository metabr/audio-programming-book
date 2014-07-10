myplug.vst: myplug
	mkdir -p myplug.vst/Contents/MacOS
	cp myplug myplug.vst/Contents/MacOS
	cp info.plist myplug.vst/Contents
	echo "BNDL????" > myplug.vst/Contents/PkgInfo
	SetFile -a B myplug.vst

myplug: myplug.cpp myplug.h
	g++ -bundle -m32 -o myplug myplug.cpp vstsdk2.4/public.sdk/source/vst2.x/audioeffect.cpp vstsdk2.4/public.sdk/source/vst2.x/audioeffectx.cpp vstsdk2.4/public.sdk/source/vst2.x/vstplugmain.cpp -I vstsdk2.4

clean:
	rm -f myplug
	rm -rf myplug.vst
