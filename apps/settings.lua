--This is the first duo app
--This is the Settings app!
settings = {};
app_name = "Settings"
app_desc = "A simple app to change settings."
app_version = 1.0
app_icon = "apps/sampleapp.png"

controls = {}
selected_control_id = 2

function settings:init()
	app_allowbgcomposition = true
	assert(require("scripts.ui.UIWindow")) 
	assert(require("scripts.ui.UIButton")) 
	
	controls[1] = UIWindow:new{}
	controls[1]:setPosition(20,30)
	
	controls[2] = UIButton:new{}
	controls[2]:setPosition(55,30)
end

function settings:update()
	drawControls(controls)
	render(false)
end

function settings:buttonPress(btn)
	if btn == 6 then app_close() 
	elseif btn == 0 then controls[selected_control_id]:event()
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