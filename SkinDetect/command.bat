set SkinPath="E:\projects\SkinDetect\codes\trunk\SkinDetect"

rem train
%SkinPath%\Release\SkinDetectBayes.exe train "%SkinPath%\SkinTrainData\*.jpg" "%SkinPath%\SkinBayes_morePic.model"

rem test
rem %SkinPath%\Release\SkinDetectBayes.exe test "E:\projects\SkinDetect\400\*.jpg" "%SkinPath%\SkinBayes.model" 0.5

rem %SkinPath%\Release\SkinDetectBayes.exe test "%SkinPath%\TestData\*.jpg" "%SkinPath%\SkinBayes.model" 3.0


