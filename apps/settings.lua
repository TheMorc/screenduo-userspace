--This is the first duo app
--This is the Settings app!
app = {};
app_name = "Settings"
app_desc = "A simple app to change settings."

x = 0
y = 0

dimscreen(25)

function app:update()

	x = x + 1
	putchar(122, x, 48 + math.floor((os.date("%H")/10)%10), 255, 255, 255); --first number
	--putchar(130, x, 48 + math.floor((os.date("%H"))%10), 255, 255, 255); --second number
	--putchar(143, x, 48 + math.floor((os.date("%M")/10)%10), 255, 255, 255); --third number
	--putchar(151, x, 48 + math.floor((os.date("%M"))%10), 255, 255, 255); --forth number
	render(false)
	--print(x)
end

function app:buttonPress(btn)
	if btn == 6 then
		app_close()
    end
end

function app:getName()
	return app_name
end

function app:getDesc()
	return app_desc
end

return app