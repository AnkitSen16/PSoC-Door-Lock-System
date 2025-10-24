function KeypadGUI_code
port = "COM9"; baud = 9600; s = [];
try s = serialport(port, baud); catch, s = []; end

f = figure('Name','Serial Keypad App','Position',[500 300 420 340], ...
    'MenuBar','none','NumberTitle','off','Resize','off');

password = "";
display = uicontrol('Style','text','Position',[20 270 380 50], ...
    'FontSize',14,'HorizontalAlignment','left','BackgroundColor','white','String','');

keys = {'1','2','3'; '4','5','6'; '7','8','9'; 'C','0','E'};
btnW = 120; btnH = 50; ml = 20; mb = 20; hG = 12; vG = 12;

for i = 1:4
    for j = 1:3
        key = keys{i,j};
        x = ml + (j-1)*(btnW + hG);
        y = mb + (4-i)*(btnH + vG);
        uicontrol('Style','pushbutton','String',key,'Position',[x y btnW btnH], ...
            'FontSize',14,'Callback',@(~,~) pressKey(key));
    end
end

f.CloseRequestFcn = @(~,~) closeApp();

    function pressKey(key)
        switch key
            case 'C' % backspace
                if strlength(password) > 0
                    password = extractBefore(password, strlength(password));
                    display.String = char(password);
                    if ~isempty(s), try write(s, uint8(8), "uint8"); catch, end, end
                end
            case 'E' % enter
                if ~isempty(s), try write(s, newline, "char"); catch, end, end
                display.String = "Sent!"; drawnow; pause(1.2); display.String = ""; password = "";
            otherwise % digits
                if strlength(password) < 10
                    password = password + key;
                    display.String = char(password);
                    if ~isempty(s), try write(s, key, "char"); catch, end, end
                end
        end
    end

    function closeApp()
        try if ~isempty(s), clear s; end, end
        delete(f);
    end
end
