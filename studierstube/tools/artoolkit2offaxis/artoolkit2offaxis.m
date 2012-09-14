% input
%       camera : 3x3 matrix as output by the ARToolkit
%            x : width of video image
%            y : height of video image
% ouput
%       position : the position (not eyepointPosition) of the camera
%       size     : the size of the view plane
% notes
%
% The camera should look about like this :
% SoOffAxisCamera {
%      viewportMapping ADJUST_CAMERA
%      size [size from matlab]
%      position [position from matlab]
%      eyepointPosition 0 0 0        
%}
%
function [position,size] = ARToolkit2OffAxis(camera, x, y)

% project image corners back
invcam = inv(camera);
corner = [ 0 x 0 x ; 0 0 y y; 1 1 1 1 ];
res = invcam * corner;
imagecorners = res(1:2,:);
% calculate center
position = [sum(imagecorners(1,:)) ; sum(imagecorners(2,:))]/4;
imagevecs = imagecorners - position * [ 1 1 1 1 ];
position = [ position' -1 ]
% and extension
size = svd(imagevecs)