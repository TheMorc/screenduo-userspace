--Do anyone remember the screensaver portion of the UI?
--it is now included in it's own app!
screensaver = {};
app_name = "Screensaver"
app_desc = "a simple 7 segment screensaver type clock"
app_version = 1.0

screensaver_colon_blinking = true --set to false to disable colon blinking
screensaver_a = 0
screensaver_colon = 2

function screensaver:update()
	if screensaver_a ~= 10 then --dim animation
		screensaver_a = screensaver_a + 1
		dimscreen(25)
		render(false) --why the render is here? it is because we dont render it on the end of the ui
	else --7 segment clock, this is much much much simpler than the C++ code was
		clearscreen() --clearing the screen (mainly for the colon so it can disappear without needing to put a blank bmp there)
		
		if screensaver_colon_blinking then
			if screensaver_colon == 2 then --blinking colon
	   			puticon(129,72,"apps/screensaver/ss_c.bmp", false, true) --colon
	   			screensaver_colon = 0
	   		end
		   		screensaver_colon = screensaver_colon + 1
	   	else
	   		puticon(129,72,"apps/screensaver/ss_c.bmp", false, true) --colon
	   	end
	   	
	   	
		if math.floor((os.date("%H")/10)%10) == 0 then --if the first number is zero, draw the empty number instead
			puticon(18,72,"apps/screensaver/ss_n.bmp", false, true) --first number
		else
			puticon(18,72,"apps/screensaver/ss_" .. math.floor((os.date("%H")/10)%10) .. ".bmp", false, true) --first number
		end
		
		puticon(84,72,"apps/screensaver/ss_" .. math.floor((os.date("%H"))%10) .. ".bmp", false, true) --second number
		puticon(173,72,"apps/screensaver/ss_" .. math.floor((os.date("%M")/10)%10) .. ".bmp", false, true) --third number
		puticon(239,72,"apps/screensaver/ss_" .. math.floor((os.date("%M"))%10) .. ".bmp", false, true) --fourth number
		
		render(false) --there is a delay between animation and rendering this due to the usleep, so we render first and then sleep
		usleep(250000) --sleep interval for the blinking colon
	end
end

function screensaver:buttonPress(btn)
	if btn == 6 then
		app_close()
    end
end

function screensaver:getName()
	return app_name
end

function screensaver:getDesc()
	return app_desc
end

function screensaver:getVersion()
	return app_version
end

return screensaver