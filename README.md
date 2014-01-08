Video-Übungsaufgabe 5 (2) Einfaches Key/Blend mit DirectShow / C++

	Umsetzung:
		OpenCV 2.3.1
		C++ / g++ 4.7.3
		Xubuntu 12.04 / 64 Bit

	File:
		differenz_codierung/src/key_blend/key_blend.cpp

	Compile:	
		g++ -o key_blend key_blend.cpp `pkg-config opencv --cflags --libs`

	Run from commandline:
		 ./key_blend image_to_blend.jpg 0.5



Video-Übungsaufgabe 5 (3) Differenzcodierung
	
	Umsetzung:
		OpenCV 2.3.1
		C++ / g++ 4.7.3
		Xubuntu 12.04 / 64 Bit

	1) Pixelweise Differenz zweier Bilder im Ergebnisbild
	2) Quantisierung der Differenzen (Teilen durch Parameter Q)

		File:
			differenz_codierung/src/dif_cod/dif.cpp

		Compile:	
			g++ -o dif dif.cpp `pkg-config opencv --cflags --libs`

		Run from commandline:
			./dif video_file_to_load.avi video_to_file_to_save.avi

	3) Blockweise Differenz/Quantisierung (4x4-Blöcke)

		File:
			differenz_codierung/src/dif_cod/block_dif.cpp

		Compile:	
			g++ -o block_dif block_dif.cpp `pkg-config opencv --cflags --libs`

		Run from commandline:
			./dif video_file_to_load.avi video_to_file_to_save.avi
