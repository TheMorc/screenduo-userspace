--
-- button control (derived from UIBaseControl)
--
UIButton = UIBaseControl:new{name = "Button", text = "Button"}
enabled = true

function UIButton:draw()
	putpng(self.xPos, self.yPos, "resources/button.png")
end

function UIButton:pressEvent()
	UIBaseControl:pressEvent(self)
end
