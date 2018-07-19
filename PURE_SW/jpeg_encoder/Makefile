default: jpeg_encoder.c
	g++ jpeg_encoder.c -DC -o jpeg_encoder.exe
io: jpeg_encoder.c
	mkdir -p iofiles
	g++ jpeg_encoder.c -DC -DIODATA -o jpeg_encoder.exe
hw_image: jpeg_encoder.c
	g++ jpeg_encoder.c -DC -DCREATEJPEG -o jpeg_encoder.exe
io_hw: jpeg_encoder.c
	mkdir -p iofiles
	g++ jpeg_encoder.c -DC -DCREATEJPEG -DIODATA -o jpeg_encoder.exe
clean:
	rm -f *.o *.exe