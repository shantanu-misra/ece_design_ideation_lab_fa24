% Save LUT to file
function save_lut_to_file(lut, filename)
    fid = fopen(filename, 'w');
    fprintf(fid, 'uint32_t Wave_LUT[%d] = {\n    ', length(lut));
    for i = 1:length(lut)
        fprintf(fid, '%d', lut(i));
        if i ~= length(lut)
            fprintf(fid, ', ');
        end
        if mod(i, 15) == 0 && i ~= length(lut)
            fprintf(fid, '\n    ');
        end
    end
    fprintf(fid, '\n};\n');
    fclose(fid);
end