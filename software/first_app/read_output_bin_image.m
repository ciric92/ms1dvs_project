clear all
close all
clc

fp = fopen('output_image.bin', 'rb');

fractional_bits = 5;

width = fread(fp, 1, 'uint32')
height = fread(fp, 1, 'uint32')

J = reshape(fread(fp, width*height, 'uint32') / 2^11, [height width])';

figure; imshow(uint8(J));