--utils.lua used to specify additions functions for app loading, ui things, etc

--app functions and variables below, you can customize and modify the app module behaviour for your own purposes
--assert(require("scripts.app")) 
app_opened = false
app_allowbgcomposition = false
app_class = "" --class name of app
app = nil --selected app
apps = {} --app table
app_count = 0 --app count

--use this function to assign and open an app
function app_open(appname)
	app_opened = true
	app_class = appname
	app = assert(require(appname))
	if logINFO then
		print("[screenduo-utils] opening " .. appname .. " (" .. app:getName() .. ")")
	end
	app:init()
end

--this is usually called from the app to close and unload the app
function app_close()
	if logINFO then
		print("[screenduo-utils] closing " .. app_class .. " (" .. app:getName() .. ")")
	end
	app_opened = false --tell UI function to "close" app
	app_allowbgcomposition = false
	package.loaded[app_class] = nil
	_G[app_class] = nil
end

--apps_fetch function to load all apps into app table for UI
package.cpath = 'scripts/?.lua;'
function apps_fetch(folder)
	package.cpath = folder .. '?.lua;' .. package.cpath --reconfigure the search path to include lua from this folder
	if logINFO then
		print("[screenduo-utils] loading apps from '" .. folder .. "'")
	end
	local f = io.popen("ls ".. folder .."*.lua | head") --macOS(Unix/Linux compatible) directory listing | needs to be fixed for Windows
 	for filename in f:lines() do
    	local class = filename:sub(1,filename:len()-4)
    	if logINFO then
			print("[screenduo-utils] registering " .. class .. " ")
    	end
    	fetchedapp = require(class)
    	app_count = app_count + 1
 		apps[app_count]={["class"]=class,["name"]=fetchedapp:getName(), ["desc"]=fetchedapp:getDesc(), ["version"]=fetchedapp:getVersion(), ["icon"]=fetchedapp:getIcon()}
 		if logINFO then
			print("[screenduo-utils] (" .. fetchedapp:getName() .. " v" .. fetchedapp:getVersion() .. ") registered sucessfully")
		end
		app_class = ""
		package.loaded[class] = nil
		_G[class] = nil
 	end
 	if logINFO then
 		print("[screenduo-utils] " .. app_count .. " apps registered successfully")
 	end
end

function controlCount(T)
  local count = 0
  for _ in pairs(T) do count = count + 1 end
  return count
end

function drawControls(table)
	local Id = 0
  	for _ in pairs(table) do
  		Id = Id + 1
  		table[Id]:draw()
  	end
end

function puttext(x,y, text, r, g, b)
	--putchar(122, 2, 48, 255, 255, 255); --x, y, letter, r, g, b color(0-255)
	for i = 1, #text do
    local c = text:sub(i,i)
    	putchar(x+(i-1)*8,y, string.byte(c), r, g, b)
	end	
end

function puttextxl(x,y, text, r, g, b)
	--putchar(122, 2, 48, 255, 255, 255); --x, y, letter, r, g, b color(0-255)
	for i = 1, #text do
    local c = text:sub(i,i)
    	putcharxl(x+(i-1)*8,y, string.byte(c), r, g, b)
	end	
end

function remap(value, low1, high1, low2, high2)
	return low2 + (value - low1) * (high2 - low2) / (high1 - low1)
end