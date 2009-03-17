@echo off
set SkinPath="E:\projects\SkinDetect\codes\trunk\SkinDetect"
set ResultPath="E:\pic_skindetect\result"

rem train
rem %SkinPath%\Release\SkinDetectBayes.exe train "%ResultPath%\SkinTrainData\*.jpg" "%ResultPath%\SkinBayes_morePic.model"

rem test
%SkinPath%\Release\SkinDetectBayes.exe test "%ResultPath%\SkinTestData\skin\*.jpg" "%ResultPath%\SkinBayes_morePic.model" "%ResultPath%\ResultData\skin0.8" 0.8

%SkinPath%\Release\SkinDetectBayes.exe test "%ResultPath%\SkinTestData\head\*.jpg" "%ResultPath%\SkinBayes_morePic.model" "%ResultPath%\ResultData\head0.8" 0.8

%SkinPath%\Release\SkinDetectBayes.exe test "%ResultPath%\SkinTestData\scene\*.jpg" "%ResultPath%\SkinBayes_morePic.model" "%ResultPath%\ResultData\scene0.8" 0.8

