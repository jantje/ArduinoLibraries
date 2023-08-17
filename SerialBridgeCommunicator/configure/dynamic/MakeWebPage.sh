#!/bin/ash 
. ./SetGlobalSettings.sh
export DumpFile=${DumpFile}
echo "<!--Start ${BeginHtml} -->"
sed 's#SKETCHNAME#'"maairobot"'#' ${BeginHtml} | sed 's#STATICFOLDER#'"${staticfolder}"'#'
echo "<!--Start MakeWebPage.lua-->"
./MakeWebPage.lua
echo "<!--Start ${EndHtml}-->"
cat ${EndHtml}
