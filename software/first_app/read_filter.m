fp = fopen('filter_coeffs.bin', 'rb');

fractional_bits = 11;

width = 7;
height = 7;

J = reshape(fread(fp, width*height, 'int16'), [width height])'

J = J/(2^fractional_bits);