--
-- base control
--
UIBaseControl = {xPos = 0, yPos = 0, name = "Control", text = "Control"}

function UIBaseControl:new(o)
	o = o or {}
    setmetatable(o, self)
    self.__index = self
    if logINFO then
    	print("[screenduo] adding " .. o.name .. " to the list of controls")
    end
    return o
end

function UIBaseControl:draw()
end

function UIBaseControl:event()
	if logINFO then
    	print("[screenduo] press event called on " .. self.name .. " (".. self.text .. ")")
    end
end

function UIBaseControl:setPosition(x, y)
	self.xPos = x
	self.yPos = y
end
