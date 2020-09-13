--This is the first duo app
--This is the Settings app!
settings = {};
app_name = "Settings"
app_desc = "A simple app to change settings."
app_version = 1.0
app_icon = "apps/sampleapp.png"



putpng(30,20,"resources/window.png")
	
function settings:update()

	--putpng(30,20,"resources/window.png")
	render(false)
end

function settings:buttonPress(btn)
	if btn == 6 then
		app_close()
    end
end

function settings:getName()
	return app_name
end

function settings:getDesc()
	return app_desc
end

function settings:getVersion()
	return app_version
end

function settings:getIcon()
	return app_icon
end

return settings