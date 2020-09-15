--
-- Window control (derived from UIBaseControl)
--
UIWindow = UIBaseControl:new{name = "Window", text = "Window text"}

function UIWindow:draw()
	putpng(self.xPos, self.yPos, "resources/window.png")
end