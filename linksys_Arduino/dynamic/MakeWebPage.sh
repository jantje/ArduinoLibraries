#!/bin/ash 
#. _DEBUG.sh
#These settings are environment dependent.
# use 1 for linksys 0 for NAS
let NumberOfLineTerminatorsToSkip=0
#end of environment specific section


BeginHtml=input.begin.html
EndHtml=input.end.html
PAGENUM=0
SPACE=" "
NUM=0
PREFDEPTH=0
PREFCLASSNAME=""
ACTIVABLES='<script src="activatables.js" type="text/javascript"></script>\n<script type="text/javascript">'
                    
                    

WrapSection()
{
let parent=WRAPDEPTH-1
eval parentclass=\${CLASSNAME${parent}}
  MADETOC="<div class=\"contentGroup\" id=\"_${parentclass}\">\n"
  MADETOC=${MADETOC}"<ol id=\"toc\">\n"
  ACTIVABLE="activatables('activable${PAGENUM}', ["
  SEP=""
  index=0
  eval sentence="\${G_SECTIONS${WRAPDEPTH}}"
  eval "G_SECTIONS${WRAPDEPTH}=\"\""
  for word in $sentence
  do
    CLASSNAMESMALL=`echo $word | sed 's/^.*\.//' `
    MADETOC=${MADETOC}"<li><a href=\"#${word}\"><span>${CLASSNAMESMALL}</span></a></li>\n"
    ACTIVABLE="${ACTIVABLE}${SEP}'${word}'"
    SEP=","
    let index+=1
  done
  MADETOC=${MADETOC}"</ol>\n"
  let PAGENUM+=1
  if [ $index -gt 0 ]; then
	#MADETOC="\n// begin wrapsection children ${parentclass}<br>\n"${MADETOC}
    eval G_Page${WRAPDEPTH}="\$MADETOC\$G_Page${WRAPDEPTH}'</div>\n'"
    #eval G_Page${WRAPDEPTH}="\$G_Page${WRAPDEPTH}'\n// end wrapsection children ${parentclass}<br>\n'"
    ACTIVABLES="${ACTIVABLES}\n${ACTIVABLE}]);"
  fi
}

WrapClass()
{
  let depth=LAST_OBJECT_DEPTH
  if [ "$depth" -lt "$PREFDEPTH" ]; then
	let depth=PREFDEPTH
  fi
  while [ $depth -gt $CURDEPTH ]; do
    let WRAPDEPTH=depth+1
    WrapSection
    eval curclass=\${CLASSNAME${depth}}
    eval AddLine="\${G_Fieldinfo${depth}}"
    if [ "$curclass" != "" ]; then
      pref=""
      #pref="\n// wrapping class ${curclass} on depth ${depth}<br>\n"${pref}
      if [ "$AddLine" = "" ]; then
        pref="${pref}<div class=\"content\"id=\"${curclass}\">\n"
      else
    	pref="${pref}<div class=\"content\"id=\"${curclass}\">\n<table>\n<tr><td>param</td><td>value</td></tr>\n$AddLine</table>\n"
      fi
      eval G_Page${depth}="\$G_Page${depth}\${pref}\${G_Page${WRAPDEPTH}}'</div>\n'"
      #eval G_Page${depth}="\$G_Page${depth}'\n// end wrapping class '\${curclass}'<br>\n'"
      eval G_Fieldinfo${depth}=""
      eval CLASSNAME${depth}=""
      eval G_Page${WRAPDEPTH}="'""'"
    fi
    let depth=depth-1
  done
}

parseLine()
{
# change NumberOfLineTerminatorsToSkip at the top of the script
# if the input lines seem to be cut
  let tmp=0
  while [ $tmp -lt $NumberOfLineTerminatorsToSkip ]
  do
    let tmp+=1
    line=${line%%?} #this is needed on openwrt but not on my nas
  done
              
  FIRST=`echo $line | cut -d' ' -f1`
  VALUE=`echo $line | cut -d' ' -f2`
}

while read line
do
  parseLine
  if [ "$FIRST" = "name" ]; then
    let NUM=NUM+1
    CLASSNAME=`echo $VALUE | sed 's/\.[^.]*$//g' `
    FIELDNAME=`echo $VALUE | sed 's/^.*\.//' `
    FULLFIELDNAME=$VALUE
    LAST_OBJECT_DEPTH=`echo $CLASSNAME|sed 's/[^.]//g'|wc -c| tr -d ' '`
    CURDEPTH=0
    if [ "$CLASSNAME" != "$PREFCLASSNAME" ] ; then
        #  echo "new class " $CLASSNAME " from " $PREFCLASSNAME
      let PREFDEPTH2=PREFDEPTH  
      let CURDEPTH=0
        
      if [ "" != "$PREFCLASSNAME" ]; then 
          let CURDEPTH=${LAST_OBJECT_DEPTH}
        matchCur=${CLASSNAME}
        matchPref=${PREFCLASSNAME}
        while [ "$CURDEPTH" -gt "$PREFDEPTH" ]; do
          let CURDEPTH-=1
          matchCur=`echo $matchCur | sed 's/\.[^.]*$//g' `
        done
        while [ "$PREFDEPTH2" -gt "$CURDEPTH" ]; do
          let PREFDEPTH2-=1
          matchPref=`echo $matchPref | sed 's/\.[^.]*$//g' `
        done
        while [ "$matchCur" != "$matchPref" ]; do
          let CURDEPTH-=1
          matchPref=`echo $matchPref | sed 's/\.[^.]*$//g' `
          matchCur=`echo $matchCur | sed 's/\.[^.]*$//g' `
          if [ $CURDEPTH -eq 0 ]; then
            matchPref=""
            matchCur=""
          fi
        done
        WrapClass
      fi    
      let LOOPDEPTH=${LAST_OBJECT_DEPTH} 
      matchCur=${CLASSNAME}
      while [ "$LOOPDEPTH" -gt "${CURDEPTH}" ]; do
        eval CLASSNAME${LOOPDEPTH}=$matchCur
        eval G_SECTIONS${LOOPDEPTH}="\${G_SECTIONS${LOOPDEPTH}}\${SPACE}\${CLASSNAME${LOOPDEPTH}}"
        matchCur=`echo $matchCur | sed 's/\.[^.]*$//g' `
        let LOOPDEPTH-=1
        #      echo ${CLASSNAME} ${LOOPDEPTH} $matchCur
      done
      let PREFDEPTH=LAST_OBJECT_DEPTH		
      let CURDEPTH=LAST_OBJECT_DEPTH
      PREFCLASSNAME=$CLASSNAME
    fi
  elif [ "$FIRST" = "SketchName" ]; then
    SKETCHNAME="$VALUE"
  elif [ "$FIRST" = "CompileDate" ]; then
    COMPILEDATE="$line"
  elif [ "$FIRST" = "modflag" ]; then
    MODLAG=$VALUE
    WRITABLE=$(($VALUE & 1))
  elif [ "$FIRST" = "Type" ]; then
    TYPE=$VALUE
  elif [ "$FIRST" = "Value" ]; then
    AddLine="<tr><td><img src='${MODLAG}.png'> ${FIELDNAME}</td><td><div id=\"${NUM}\"></div></td>"
    if [ "$WRITABLE" -eq "1" ]; then
      AddLine="${AddLine}<td><input type=\"${TYPE}\"id=\"t_${NUM}\"></td><td><button type=\"button\" onclick=\"UpdateValue(${NUM},'${FULLFIELDNAME}');\">set</button></td>"
    fi
    AddLine="${AddLine}</tr>\n"
    eval G_Fieldinfo${LAST_OBJECT_DEPTH}="\$G_Fieldinfo${LAST_OBJECT_DEPTH}\$AddLine"
#    echo
#    echo "<br>-- dumping object for class ${CLASSNAME0} at depth 0 <br>"
#    echo "${G_Fieldinfo0}"
#    echo "<br>-- end dumping--<br>"
#    echo 
#    echo
#    echo "<br>-- dumping object for class ${CLASSNAME1} at depth 1 <br>"
#    echo "${G_Fieldinfo1}"
#    echo "<br>-- end dumping--<br>"
#    echo 
#    echo
#    echo "<br>-- dumping object for class ${CLASSNAME2} at depth 2 <br>"
#    echo "${G_Fieldinfo2}"
#    echo "<br>-- end dumping--<br>"
#    echo 
#    echo
#    echo "<br>-- dumping object for class ${CLASSNAME3} at depth 3 <br>"
#    echo "${G_Fieldinfo3}"                
#    echo "<br>-- end dumping--<br>"
#    echo 
#    echo
#    echo "<br>-- dumping object for class ${CLASSNAME4} at depth 4 <br>"
#    echo "${G_Fieldinfo4}"
#    echo "<br>-- end dumping--<br>"
#    echo 
  fi
done


#now it is time to write everything out
     CURDEPTH=-1
    WrapClass
      sed 's#SKETCHNAME#'"${SKETCHNAME}"'#' input.begin.html 
      echo -e "$G_Page0" 
      echo -e "$G_Page1"
                                             
      echo -e "${COMPILEDATE}\n" 
      echo -e "<a href=\"/cgi-bin/robot/cgiutil/upload.lua\">admin page </a>"
      echo -e "\n<!--Start activables-->\n"
      echo -e "$ACTIVABLES"  
      echo "</script>" 
      echo -e "\n<!--Start $EndHtml-->\n" 

