--
-- sample app
--
sampleapp = {};
app_name = "Sample app"
app_desc = "A sample app."
app_version = 1.0


function sampleapp:update()
end

function sampleapp:buttonPress(btn)
end

function sampleapp:getName()
	return app_name
end

function sampleapp:getDesc()
	return app_desc
end

function sampleapp:getVersion()
	return app_version
end

return sampleapp