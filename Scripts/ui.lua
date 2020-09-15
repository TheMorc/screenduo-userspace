--this is a barebones LUA UI setup, all of the original main.cpp UI code should be converted and merged here
--so that main.cpp is not needed to be recompiled every single time you want to change a single damn thing..

--Working lua gl functions
   	--fillscreen(25) --args: color(0-255)
	--putpixel(1+x, 1+y, 0, 255, 255) --args: x, y, r, g, b color(0-255)
	--putpixelxl(1, 1, 0, 255, 255) --args: x, y, r, g, b color(0-255)
	--dimscreen(25) --args: amount
	--puticon(239,20,"Resources/button.bmp", true, true) --args: x, y, filename, transparency, compatibility
	--putchar(122, 2, 48, 255, 255, 255); --x, y, letter, r, g, b color(0-255)
assert(require("scripts.utils")) --load functions from scripts/utils.lua | mandatory for apps_fetch and for ui things
assert(require("scripts.settings")) --load all settings from scripts/settings.lua | mandatory for logINFO to not show log when disabled and app1/app2 buttons to work
assert(require("scripts.ui.UIBaseControl")) 
	assert(require("scripts.ui.UIWindow")) 
	assert(require("scripts.ui.UIButton")) 


--UI variables
colon_blinking = true --set to false to disable colon blinking
colon = 2
app_index = 1 --current selected app

--this is a preUI load stage that was also used in main.cpp before but left unused
function preUI()
	--putpngbg("apps/sampleapp.png")
	--render(false)
end

apps_fetch("apps/") --load apps from specific folder

--this is the main UI function that is being looped in the C++ code!
function UI()
	if app_allowbgcomposition then --if is an app opened, go straight into the app UI function and do not proceed into the UI function
		app:update()
	elseif app_opened then
		app:update()
		return
	end
	
	putpng(0,0,"resources/ui_background.png") --used for clearing the screen
	
	--putpng(30,20,"resources/window.png")
	
	
	local app_ = 0
	for key in pairs(apps) do
  		app_ = app_ + 1
  		--putchar(20+(app_*8), 50, 48, 255, 255, 255)
  		putpng((app_*45)-18,195,apps[key].icon)
	end
	
	--app selector
  	putpng((app_index*45)-4,229,"resources/selector.png")
	
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

	if not app_allowbgcomposition then --if is an app opened, go straight into the app UI function and do not proceed into the UI function
		render(false)
	end
	--app_open("apps/settings'")
end

--button event function executed after every successful button press either using SDL or libusb
function buttonPress(btn)
    --lua button press print
    if logINFO then
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
    end
    
    if app_opened then --check for opened app, if not go to ui buttons
		app:buttonPress(btn)
		return
	end
        
    --if btn == 12 then screensaver = not screensaver    screensaver_a = 0
    if btn == 1 then if app_index ~= 1 then app_index = app_index - 1 else app_index = app_count end
    elseif btn == 2 then if app_index ~= app_count then app_index = app_index + 1 else app_index = 1 end
    elseif btn == 0 then app_open(apps[app_index].class)
    elseif btn == 12 then app_open(app1) --open app from app1 located in settings.lua
    elseif btn == 13 then app_open(app2) --open app from app2 located in settings.lua
    end
end
