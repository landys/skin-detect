@echo off
set SkinPath="E:\projects\SkinDetect\codes\trunk\SkinDetect"
set ResultPath="E:\pic_skindetect\result"

rem train
rem %SkinPath%\Release\SkinDetectBayes.exe train "%ResultPath%\SkinTrainData\*.jpg" "%ResultPath%\SkinBayes_morePic.model"

rem test
%SkinPath%\Release\SkinDetectBayes.exe test "%ResultPath%\SkinTestData\skin\*.jpg" "%ResultPath%\SkinBayes_morePic.model" "%ResultPath%\ResultData\skin2" 1.0

rem %SkinPath%\Release\SkinDetectBayes.exe test "%ResultPath%\SkinTestData\head\*.jpg" "%ResultPath%\SkinBayes_morePic.model" "%ResultPath%\ResultData\head" 1.0

rem %SkinPath%\Release\SkinDetectBayes.exe test "%ResultPath%\SkinTestData\scene\*.jpg" "%ResultPath%\SkinBayes_morePic.model" "%ResultPath%\ResultData\scene" 1.0

