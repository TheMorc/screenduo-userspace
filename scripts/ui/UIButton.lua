--
-- button control (derived from UIBaseControl)
--
UIButton = UIBaseControl:new{name = "Button", text = "Button"}

function UIButton:draw()
	putpng(self.xPos, self.yPos, "resources/selector.png")
end

function UIButton:event()
	UIBaseControl:event()
end
