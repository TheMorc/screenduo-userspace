--
-- base control
--
UIBaseControl = {xPos = 0, yPos = 0, name = "Control", text = "Control"}

function UIBaseControl:new(o)
	o = o or {}
    setmetatable(o, self)
    self.__index = self
    if logINFO then
    	print("[screenduo-ui] adding " .. o.name .. " to the list of controls")
    end
    return o
end

function UIBaseControl:draw()
end

function UIBaseControl:pressEvent(oo)
	oo = oo or self
	if logINFO then
    	print("[screenduo-ui] press event called on " .. oo.name .. " (".. oo.text .. ")")
    end
end

function UIBaseControl:focusEvent(oo)
	oo = oo or self
	if logINFO then
    	print("[screenduo-ui] focus event called on " .. oo.name .. " (".. oo.text .. ")")
    end
end

function UIBaseControl:setPosition(x, y)
	self.xPos = x
	self.yPos = y
end
