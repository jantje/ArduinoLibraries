#! /usr/bin/lua

overviewName ="Overview"

--reads the input and puts it into memory
function readInput()
  local DumpFile=os.getenv("DumpFile")
  if (DumpFile==nil) then  --fix for lua issue? http://www.gammon.com.au/forum/?id=12881
    DumpFile="/tmp/jantje/ttyATH0/dump.txt"
  end
  local inp = io.open(DumpFile,"r")
  local curfield={}
  local curField=0
  for line in inp:lines() do
    --for line in lines do
    if not (line == "") then
      --print (line)
      local FIRST
      local VALUE
      _, _, FIRST, VALUE = string.find(line, "(%w+)%s+(.+)")
      if VALUE==nil then
        VALUE=""
        FIRST=line
      end
      --this should only happen once
      if FIRST == "SketchName"  then
        SKETCHNAME=VALUE
        --this should only happen once
      elseif FIRST == "CompileDate"  then
        COMPILEDATE=line
      elseif FIRST == "Dumping"  then
      --ignore this line
      else

        curfield[FIRST]=VALUE
        --value is the last field so we need to add the field to the tree
        if FIRST == "Value"  then
          curField=curField+1
          curfield.index=curField
          curfield.fieldName=string.sub(curfield.name,string.find(curfield.name,"[^.]*$"))
          local _, countDots=  string.gsub(curfield.name, "%.", "")
          local parent=allfields
          local parents=curfield.name
          for curparent=1, countDots do
            curparent = string.sub(parents,string.find(parents,"^%w*"))
            parents=string.sub(parents,string.len(curparent)+2,-1)
            if parent[curparent]==nill then
              --table.insert(parent, curparent)
              parent[curparent]={}
              parent[curparent].index=curField
            end
            parent=parent[curparent]
          end

          parent[curfield.fieldName]=curfield
          curfield={}
          parent={}
        end
      end
    end
  end
end


function makeItemLine(curfield,type)
  local retString=""
  local imgnum= myand(curfield.mod, 15)
  local name=curfield.fieldName
  local prefix =""
  if (type==2) then
     name=curfield.name
     prefix ="o_"
  end
  retString= retString .."<tr><td><img src='/jantje/"..imgnum..".png'>".. name.."</td>"
  retString= retString .."<td><div id='"..prefix..curfield.index.."'></div></td>"
  --local writable = (math.fmod(curfield.mod,2)==1)--this is a workaround for boolean and 1 because in lua 5.1 there is no boolean logic
  local writable = ( myand(curfield.mod, 1)==1)--this is a workaround for boolean and 1 because in lua 5.1 there is no boolean logic
  if (writable) then
    retString= retString ..'<td><input type="'..curfield.Type..'" id="t_' ..curfield.index..'"></td>'
    retString= retString ..'<td><button type="button" onclick="UpdateValue(' ..curfield.index..','.."'"..curfield.name.."'"..');">set</button></td>\n</tr>\n'
  else
    retString= retString ..'<td></td></tr>\n'
  end
  return retString
end

overviewLines={}
maxOverviewIndex=0

function makeContent(startField,parent)
  local contentgrouphtml= ""
  local fieldLines={}
  local childLines={}
  local contengroupLines={}
  local finishLine=""


  if(type(startField)~="table")then
    print ("Something wrong")
    print (startField)
  else
    local maxFiedindex=1
    local maxChildindex=1
    for key,curfield in pairs(startField) do
      if type(curfield)=="table" then

        if curfield.mod==nil then
          if childLines[curfield.index]==nill then
            childLines[curfield.index]=""
            contengroupLines[curfield.index]=""
          end

          contengroupLines[curfield.index]=contengroupLines[curfield.index]..'<li><a href="#'..parent..key ..'"><span>'..key ..'</span></a></li>\n'
          childLines[curfield.index]= childLines[curfield.index] ..'<div class="content" id="'..parent..key..'">\n'
          childLines[curfield.index]= childLines[curfield.index] .. makeContent(curfield,parent..key..".")
          childLines[curfield.index]= childLines[curfield.index] .. '</div>\n'
          maxChildindex=math.max(maxChildindex,curfield.index)
        else
          fieldLines[curfield.index]= makeItemLine(curfield,1)
          if (myand(curfield.mod, 64)==64)then
            overviewLines[curfield.index] =makeItemLine(curfield,2)
            maxOverviewIndex=math.max(maxOverviewIndex,curfield.index)
          end
          maxFiedindex=math.max(maxFiedindex,curfield.index)
        end

      end
    end

    if maxFiedindex >1 then
      contentgrouphtml= contentgrouphtml.."<table>\n"
      contentgrouphtml= contentgrouphtml.."<tr><td>param</td><td>value</td></tr>\n"
      for curline=1,maxFiedindex do
        if (fieldLines[curline]~=nil) then
          contentgrouphtml=contentgrouphtml..fieldLines[curline]
        end
      end
      contentgrouphtml = contentgrouphtml.. '</table>\n'
    end
    if maxChildindex >1 then
      contentgrouphtml=contentgrouphtml..'<div class="contentGroup" id="_'..parent..'">\n'
      contentgrouphtml= contentgrouphtml..'\n<ol id="toc">\n'
      for curline=1,maxChildindex do
        if (childLines[curline]~=nil) then
          contentgrouphtml=contentgrouphtml..contengroupLines[curline]
        end
      end
      if(parent=="") then
        contentgrouphtml=contentgrouphtml..'<li><a href="#'..overviewName..'"><span>'..overviewName..'</span></a></li>\n'
      end
      contentgrouphtml= contentgrouphtml .. '</ol>\n'
      finishLine= finishLine .. '</div>\n'
    end
    if maxChildindex >1 then
      for curline=1,maxChildindex do
        if (contengroupLines[curline]~=nil) then
          contentgrouphtml=contentgrouphtml..childLines[curline]
        end
      end
    end
    if(parent=="") then
      contentgrouphtml=contentgrouphtml..'<div class="content" id="'..overviewName..'">\n'
      contentgrouphtml= contentgrouphtml.."<table>\n"
      contentgrouphtml= contentgrouphtml.."<tr><td>param</td><td>value</td></tr>\n"
      for curline=1,maxOverviewIndex do
        if (overviewLines[curline]~=nil) then
          contentgrouphtml=contentgrouphtml..overviewLines[curline]
        end
      end
      contentgrouphtml = contentgrouphtml.. '</table>\n'
      finishLine= finishLine .. '</div>\n'
    end
  end

  return contentgrouphtml..finishLine
end

function makeActivableScriptsrecurse(startField, keyPrefix)
  local activablescripts= ""
  local activables=""
  local activablesPrefix="'"
  local found =false
  for key,curfield in pairs(startField) do
    if type(curfield)=="table" then
      if curfield.mod==nil then
        activables= activables .. activablesPrefix..keyPrefix..key.."'"
        activablesPrefix=",'"
        found=true
        activablescripts=activablescripts..makeActivableScriptsrecurse(curfield,keyPrefix..key..".")
      end
    end
  end
  if found then
    if (keyPrefix=="")then
      if(maxOverviewIndex>0)then
        activables=activables..activablesPrefix..overviewName.."'"
        CurActivable=CurActivable+1
      end
    end
    activablescripts=activablescripts.."\nactivatables('activable"..CurActivable.."', ["..activables.."]);"
    CurActivable=CurActivable+1
  end
  return activablescripts
end

function makeActivableScripts()
  local result='<script src="/jantje/activatables.js" type="text/javascript"></script>\n'
  local result=result ..'<script type="text/javascript">\n'
  CurActivable=1
  result=result..makeActivableScriptsrecurse(allfields,"")
--  if(numOverviewLines>0)then
--    result=result.."\nactivatables('activable"..CurActivable.."', ["..overviewName.."]);"
--    CurActivable=CurActivable+1
--  end
  result=result ..'\n</script>\n'
  return result
end


local function nand(x,y,z)
  z=z or 2^8
  if z<2 then
    return 1-x*y
  else
    return nand((x-x%z)/z,(y-y%z)/z,math.sqrt(z))*z+nand(x%z,y%z,math.sqrt(z))
  end
end
math["not"]=function(y,z)
  return nand(nand(0,0,z),y,z)
end
math["and"]=function(x,y,z)
  return nand(math["not"](0,z),nand(x,y,z),z)
end
math["or"]=function(x,y,z)
  return nand(math["not"](x,z),math["not"](y,z),z)
end
math["xor"]=function(x,y,z)
  return math["and"](nand(x,y,z),math["or"](x,y,z),z)
end

function myand(x, y)
  return math["and"](x,y)
end





allfields={}

readInput()
html=""
--html = makeMainContentGroup()
html = html..makeContent(allfields,"")
html=html..makeActivableScripts()
html=html .. COMPILEDATE
html=html .. '\n<a href="upload.lua">admin page </a>'

print(html)

     

