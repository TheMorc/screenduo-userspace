--
-- base app
--
App = {name = "App", desc = "App description", version = 1.0, icon = "resources/app.png"}

function App:new(o)
	o = o or {}
    setmetatable(o, self)
    self.__index = self
    return o
end

function App:init()
end

function App:update()
end

function App:buttonPress(btn)
end

function App:getName()
	return self.name
end

function App:getDesc()
	return self.desc
end

function App:getVersion()
	return self.version
end

function App:getIcon()
	return self.icon
end