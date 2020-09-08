--this is a barebones LUA UI setup, all of the original main.cpp UI code should be converted and merged here
--so that main.cpp is not needed to be recompiled every single time you want to change a single damn thing..

--Working lua gl functions
   	--fillscreen(25) --args: color(0-255)
	--putpixel(1+x, 1+y, 0, 255, 255) --args: x, y, r, g, b color(0-255)
	--putpixelxl(1, 1, 0, 255, 255) --args: x, y, r, g, b color(0-255)
	--dimscreen(25) --args: amount
	--puticon(239,20,"Resources/button.bmp", true, true) --args: x, y, filename, transparency, compatibility
	--putchar(122, 2, 48, 255, 255, 255); --x, y, letter, r, g, b color(0-255)

--7segment clock screensaver variables
screensaver_colon_blinking = true --set to false to disable colon blinking
screensaver = false --set to true to enable screensaver automatically
screensaver_a = 0
screensaver_colon = 2

function preUI() --this is a preUI load stage that was also used in main.cpp before
	putbg("Resources/ui_background.bmp")
	render(false, false)
end

function UI() --this is the main UI function that is looped in the C++ code!
	
	--screensaver = true --enabling screensaver for testing purposes
	--print(collectgarbage("count"))
	
	--this is a recreation of the loop from main.cpp
	if screensaver then
		if screensaver_a ~= 10 then --dim animation
			screensaver_a = screensaver_a + 1
			dimscreen(25)
			render(false, false) --why the render is here? it is because we dont render it on the end of the ui
		else --7 segment clock, this is much much much simpler than the C++ code was
			clearscreen() --clearing the screen (mainly for the colon so it can disappear without needing to put a blank bmp there)
			
			if screensaver_colon_blinking then
				if screensaver_colon == 2 then --blinking colon
		   			puticon(129,72,"Resources/ss_c.bmp", false, true) --colon
		   			screensaver_colon = 0
		   		end
		   		screensaver_colon = screensaver_colon + 1
		   	else
		   		puticon(129,72,"Resources/ss_c.bmp", false, true) --colon
		   	end
		   	
		   	
			if math.floor((os.date("%H")/10)%10) == 0 then --if the first number is zero, draw the empty number instead
				puticon(18,72,"Resources/ss_n.bmp", false, true) --first number
			else
				puticon(18,72,"Resources/ss_" .. math.floor((os.date("%H")/10)%10) .. ".bmp", false, true) --first number
			end
			
			puticon(84,72,"Resources/ss_" .. math.floor((os.date("%H"))%10) .. ".bmp", false, true) --second number
			puticon(173,72,"Resources/ss_" .. math.floor((os.date("%M")/10)%10) .. ".bmp", false, true) --third number
			puticon(239,72,"Resources/ss_" .. math.floor((os.date("%M"))%10) .. ".bmp", false, true) --fourth number
			
			render(false, false) --there is a delay between animation and rendering this due to the usleep, so we render first and then sleep
			usleep(250000) --sleep interval for the blinking colon
		end
	else
		putbg("Resources/ui_background.bmp") --used for clearing the screen
		
		putchar(137,2,58,255,255,255) --colon
		
		putchar(122, 2, 48 + math.floor((os.date("%H")/10)%10), 255, 255, 255); --first number
		putchar(130, 2, 48 + math.floor((os.date("%H"))%10), 255, 255, 255); --second number
		putchar(143, 2, 48 + math.floor((os.date("%M")/10)%10), 255, 255, 255); --third number
		putchar(151, 2, 48 + math.floor((os.date("%M"))%10), 255, 255, 255); --forth number
	end
	
	if not screensaver then
		render(false, false) --render is omitted here due to being executed up there
	end
end

function buttonPress(btn)--button event function executed after every successful button press either using SDL or libusb
    if     btn == 3 then print(btn .. " (dpad up)")
    elseif btn == 1 then print(btn .. " (dpad left)")
    elseif btn == 2 then print(btn .. " (dpad right)")
    elseif btn == 4 then print(btn .. " (dpad down)")
    elseif btn == 6 then print(btn .. " (back)")
    elseif btn == 0 then print(btn .. " (enter)")
    elseif btn == 12 then print(btn .. " (app 1)")    screensaver = not screensaver    screensaver_a = 0
    elseif btn == 13 then print(btn .. " (app 2)")
    else print(btn .. " (?? what button is this)")
    end
end