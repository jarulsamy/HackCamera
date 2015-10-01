img = uint8(0);
for y = 1:251
    for x = 1:201
        img(y,x) = uint8(y);
    end;
end;
for y = 51:201
    for x = 51:151
        img(y,x) = uint8(x);
    end;
end;
idisp(img);