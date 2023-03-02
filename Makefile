$(CC) = gcc

final:
		$(CC) main.c compressImplementation.c decompressImplementation.c -o final
clean:
		rm *.o final