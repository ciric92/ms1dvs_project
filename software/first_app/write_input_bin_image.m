I = imread('cam.bmp');


fp = fopen('cam.bin', 'wb');

width = size(I, 2)
height = size(I, 1)

fwrite(fp, width, 'uint32');
fwrite(fp, height, 'uint32');
fwrite(fp, I', 'uint8');

fclose(fp);