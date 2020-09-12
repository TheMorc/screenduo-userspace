--this is a barebones LUA UI setup, all of the original main.cpp UI code should be converted and merged here
--so that main.cpp is not needed to be recompiled every single time you want to change a single damn thing..

--Working lua gl functions
   	--fillscreen(25) --args: color(0-255)
	--putpixel(1+x, 1+y, 0, 255, 255) --args: x, y, r, g, b color(0-255)
	--putpixelxl(1, 1, 0, 255, 255) --args: x, y, r, g, b color(0-255)
	--dimscreen(25) --args: amount
	--puticon(239,20,"Resources/button.bmp", true, true) --args: x, y, filename, transparency, compatibility
	--putchar(122, 2, 48, 255, 255, 255); --x, y, letter, r, g, b color(0-255)

--app functions and variables below, you can customize and modify the app module behaviour for your own purposes
app_opened = false
app_class = ""
app = nil

--use this function to assign and open an app
function app_open(appname)
	app_opened = true
	app_class = appname
	app = require(appname)
	print("[screenduo] opening " .. appname .. " (" .. app:getName() .. ")")
	app_update(appname)
end

--this has to be strictly called from the UI function
function app_update(filename)
	app:update()
end

--this is usually called from the app to close and unload the app
function app_close()
	print("[screenduo] closing " .. app_class .. " (" .. app:getName() .. ")")
	app_opened = false --tell UI function to "close" app
	package.loaded[app_class] = nil
	_G[app_class] = nil
end

package.cpath = 'apps/?.lua;scripts/?.lua'
apps = {}
app_count = 0
function apps_fetch()
	local f = io.popen("ls -td apps/*.lua | head") --macOS(Unix/Linux compatible) directory listing
 	for filename in f:lines() do
    	local class = filename:sub(1,filename:len()-4)
    	fetchedapp = require(class)
    	app_count = app_count + 1
 		apps[app_count]={["class"]=class,["name"]=fetchedapp:getName(), ["desc"]=fetchedapp:getDesc()}
		print("[screenduo] registering (" .. fetchedapp:getName() .. ") to app list")
		package.loaded[class] = nil
		_G[class] = nil
 	end
 	print("[screenduo] app count is " .. app_count)
end

--UI variables
colon_blinking = true --set to false to disable colon blinking
colon = 2
app_index = 1

--this is a preUI load stage that was also used in main.cpp before but left unused
function preUI()
	--putbg("Resources/ui_background.bmp")
	--render(false, false)
end

apps_fetch()

--this is the main UI function that is being looped in the C++ code!
function UI()
	if app_opened then --if is an app opened, go straight into the app UI function and do not proceed into the UI function
		app_update(app_class)
		return
	end
	
	putbg("Resources/ui_background.bmp") --used for clearing the screen
	
	local app_ = 0
	for key, class, name, desc in pairs(apps) do
  		app_ = app_ + 1
  		putchar(20+(app_*8), 50, 48, 255, 255, 255)
	end
	
	
  	putchar(20+(app_index*8), 30, 48, 255, 255, 255)
	
	if colon_blinking then
		if colon == 2 then --blinking colon
			putchar(137,2,58,255,255,255) --colon
	   		colon = 0
	   	end
	   	colon = colon + 1
	else
		putchar(137,2,58,255,255,255) --colon
	end
		
	
	putchar(122, 2, 48 + math.floor((os.date("%H")/10)%10), 255, 255, 255); --first number
	putchar(130, 2, 48 + math.floor((os.date("%H"))%10), 255, 255, 255); --second number
	putchar(143, 2, 48 + math.floor((os.date("%M")/10)%10), 255, 255, 255); --third number
	putchar(151, 2, 48 + math.floor((os.date("%M"))%10), 255, 255, 255); --fourth number

	render(false, false)
end

--button event function executed after every successful button press either using SDL or libusb
function buttonPress(btn)
    --lua button press print
    if btn == 1 then print(btn .. " (dpad left)")
    elseif btn == 2 then print(btn .. " (dpad right)")
    elseif btn == 3 then print(btn .. " (dpad up)")
    elseif btn == 4 then print(btn .. " (dpad down)")
    
    elseif btn == 6 then print(btn .. " (back)")
    elseif btn == 0 then print(btn .. " (enter)")
    
    elseif btn == 12 then print(btn .. " (app 1)")
    elseif btn == 13 then print(btn .. " (app 2)")
    else print(btn .. " (?? what button is this)")
    end
    
    if app_opened then --check for opened app, if not go to ui buttons
		app:buttonPress(btn)
		return
	end
        
    --if btn == 12 then screensaver = not screensaver    screensaver_a = 0
    if btn == 1 then if app_index ~= 1 then app_index = app_index - 1 else app_index = app_count end
    elseif btn == 2 then if app_index ~= app_count then app_index = app_index + 1 else app_index = 1 end
    elseif btn == 0 then app_open(apps[app_index].class)
    elseif btn == 12 then app_open("apps.screensaver")
    elseif btn == 13 then app_open("apps.settings")
    end
end