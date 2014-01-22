
Video-Übungsaufgabe 5 (4) Blcokdifferenz
	
	Umsetzung:
		OpenCV 2.3.1
		C++ / g++ 4.7.3
		Xubuntu 12.04 / 64 Bit

	1) Blockweise Differenz/Quantisierung (4x4-Blöcke)

		Quellcode
			differenz_codierung/src/dif_cod/block_dif.cpp

		Binärcode
			differenz_codierung/src/dif_cod/block_dif

		Kompilierung	
			g++ -o block_dif block_dif.cpp `pkg-config opencv --cflags --libs`

		Starten aus der Konsole
			./dif_block video_file_to_load.avi video_to_file_to_save.avi

	Vorgehensweise
		Die Aufgabe wurde in C++ mit dem OpenCV Framework umgesetzt und wurde vollständig bearbeitet. Der Quellcode ist unter den oben genannten Abhängigkeiten / Plattform kompilierbar und lauffähig.

		Alle Kanäle der Pixel in den Video-Frames werden mit dem Helligkeitswert bzw. dem Wert des ersten Kanals des jeweiligen Pixels überschrieben. Bei Bilddaten im YUV-Format wird der Y-Kanal als Helligkeitswert für alle drei Kanäle verwendet. Bei Daten im RGB-Format wird der B-Kanal verwendet, da OpenCV aus Performance-Gründen die Kanäle in der Reihenfolge BGR anstatt RGB verwaltet.
		Hierbei wird die Einhaltung des Wertebereichs zwischen 0 und 255 beachtet. 
		Als nächstes wird der Mittelwert der 4x4 Matrix gebildet. Hierfür werden alle Pixel bzw. Kanäle der 16 Pixel addiert und mit 16 dividiert. Das gerundete Ergebnis wird allen Kanälen der 16 Pixel als neuer Wert zugewiesen. 
		Als letztes werden der vorherige Frame vom aktuellen subtrahiert um den Differenz-Frame zu erhalten.

	Probleme / Einschränkungen
		Die blockweise Iteration über die Pixel bereitete und anfänglich ein paar Schwierigkeiten, die aber nach dem Aufzeichnen eines vereinfachten Beispiels auf Papier gelöst werden konnten.

		Die Implementierung arbeitet momentan mit einem "kontinuierlichen" Datenformat. Die Bilddaten müssen also als eindimensionales anstatt eines zweidimensionalen Arrays vorliegen.
		
		Aufgrund des hartkodierten Ausgabeformats(.avi) können nur Videos im AVI-Format verwendet werden.