--This is the first duo app
--This is the Settings app!
settings = {};
app_name = "Settings"
app_desc = "A simple app to change settings."
app_version = 1.2
app_icon = "apps/settings.png"

controls = {}
checkboxTable = {}
selected_control = 1
control_count = 0

function settings:init()
	app_allowbgcomposition = true
	
	controls[1] = UIWindow:new{}
	controls[1]:setPosition(20,0)
	controls[1].text = app_name .. " v" .. app_version
	
	controls[2] = UIButton:new{}
	controls[2]:setPosition(188,159)
	
	controls[3] = UICheckbox:new{}
	controls[3]:setPosition(80,40)
	
	controls[4] = UIRadiobutton:new{}
	controls[4]:setPosition(120,40)
	
	controls[5] = UIRadiobutton:new{}
	controls[5]:setPosition(120,60)
	
	controls[6] = UIProgressbar:new{}
	controls[6]:setPosition(38,160)
	controls[6].value = 0
	controls[6].maxValue = 100
	controls[6].width = 147
	
	control_count = controlCount(controls)
end

function settings:update()
	drawControls(controls)
	if controls[6].value ~= 100 then
		controls[6].value = controls[6].value + 1
	else
		controls[6].value = 0
	end
	render(false)
end

function settings:buttonPress(btn)
	if btn == 1 then 	controls[selected_control]:exitFocusEvent()		if selected_control ~= 1 then selected_control = selected_control - 1 else selected_control = control_count end		controls[selected_control]:enterFocusEvent()
	elseif btn == 2 then 	controls[selected_control]:exitFocusEvent()		if selected_control ~= control_count then selected_control = selected_control + 1 else selected_control = 1 end		controls[selected_control]:enterFocusEvent()
	elseif btn == 6 then app_close() 
	elseif btn == 0 then controls[selected_control]:pressEvent()
    	elseif btn == 3 then controls[1]:setPosition(controls[1].xPos, controls[1].yPos-1)
    	elseif btn == 4 then controls[1]:setPosition(controls[1].xPos, controls[1].yPos+1)
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