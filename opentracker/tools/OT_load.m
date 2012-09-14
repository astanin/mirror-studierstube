% Read Open Tracker File
FileName = 'bcidata12.dat';
fid = fopen(FileName,'r');
cnt = 0;
data=[];
while ~feof(fid)
    tline = fgetl(fid);
    pos = findstr(tline, 'float.bcidata0');
    pos1 = findstr(tline, 'float.bcidata4');
    if ~isempty(pos) & ~isempty(pos1)
    if ~isempty(pos)
        posEqu = findstr(tline(pos+15+[0:5]),'=');
        len = str2num(tline(pos+14+[1:posEqu-1]));
        tmp_data = sscanf(tline(pos+14+posEqu+[1:len]),'%f');
        tmp_time = sscanf(tline(findstr(tline,'{')+1:findstr(tline,'<')-3),'%f');
        cnt = cnt + 1;
        data(1,cnt) = tmp_time;
        data(2,cnt) = tmp_data;
    end
    pos = pos1;
    if ~isempty(pos)
        posEqu = findstr(tline(pos+15+[0:5]),'=');
        len = str2num(tline(pos+14+[1:posEqu-1]));
        tmp_data = sscanf(tline(pos+14+posEqu+[1:len]),'%f');
        data(3,cnt) = tmp_data;
    end
    end
end

plot(data(3,:))

%------------------------------------
% Read Open Tracker Raw File
FileName = 'rawlog13.dat';
fid = fopen(FileName,'r');
out= fread(fid,inf, 'int16');
fclose(fid);


plot(out(1:8:end))




%------------------------------------
% Using readOpenTracker
bcidata=readOpenTracker('bcidata2.dat');
cnt = 0;
data=[];t=[];
for k = 1 : length(bcidata)
  if ~isempty(bcidata(k).bcidata0)
    cnt = cnt + 1;
    t(cnt) = bcidata(k).timestamp;
    data(1,cnt) = bcidata(k).bcidata0;
    data(2,cnt) = bcidata(k).bcidata1;
    data(3,cnt) = bcidata(k).bcidata2;
    data(4,cnt) = bcidata(k).bcidata3;
    data(5,cnt) = bcidata(k).bcidata4;
    data(6,cnt) = bcidata(k).bcidata5;
    data(7,cnt) = bcidata(k).bcidata6;
    data(8,cnt) = bcidata(k).bcidata7;

  end
end


%----------------------------------------------
% UDP Receive
%----------------------------------------------
addpath([pwd,'\tcp_udp_ip'])
clear all
ReceivePort = 2069;
pnet('closeall')
% Receiver
ReceiveUDPHandle = pnet('udpsocket', ReceivePort);
pnet(ReceiveUDPHandle, 'setreadtimeout',0);
pnet(ReceiveUDPHandle,'status')

data=[];
cnt=0;
while 1==1
    len = pnet(ReceiveUDPHandle, 'readpacket'); 
    if len > 0   
        cnt=cnt+1;
        tmp = pnet(ReceiveUDPHandle, 'read',len,'int8');
        %disp([num2str(cnt) ' ' tmp])
        data(cnt,:)=int8(tmp);
    end
end


% Convert data
% byte 1 = Nr of Channels in bits
% byte 2-5 = counting number
% bytes   = data byte (2 per number)
chnr = data(1,1);
if chnr < 0
  chnr = chnr + 256;
end
chnr = dec2bin(chnr);
chnr = length(find(chnr=='1'));

dat=data(:,6:end);
dat1=reshape(dat',2*chnr,size(dat,1)*size(dat,2)/chnr/2)';
datnew = [];
for k = 1 : 2 : size(dat1,2)
  tmp = dat1(:,k:k+1);
  tmp = tmp(:,2)*256+tmp(:,1);
  datnew = [datnew tmp];
end

plot(datnew(:,5))

%-------------------------------------------
% ECG detection

ECG = datnew(:,5);

Th = 3000 / 10^5; % [values]
TooMuch = 20000 / 10^5;   % [values
OnDur = 4;    % [samples]
OffDur = 100;  % [samples]

over = 0; beat = []; downcnt = 0;
for k = 1 : length(ECG)
    if downcnt == 0 
      if (ECG(k) > Th) & (ECG(k) < TooMuch)
          over = over +1;
      else
          over = 0;
      end
      if over >= OnDur
          downcnt = OffDur;
          disp('beat')
          beat = [beat k];
      end
    else
      downcnt = downcnt - 1;
    end
end

figure
plot(ECG)
hold
plot(beat, ECG(beat),'rx','LineWidth',3)

