--
-- Window control (derived from UIBaseControl)
--
UIWindow = UIBaseControl:new{name = "Window", text = "Window text"}

function UIWindow:draw()
	putpng(self.xPos, self.yPos, "resources/window.png")
	puttextxl(self.xPos/2+6,self.yPos/2+4,self.text,1,1,1)
	puttextxl(self.xPos/2+7,self.yPos/2+3,self.text,255,255,255)
end