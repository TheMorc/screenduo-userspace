--
-- Window control (derived from UIBaseControl)
--
UIWindow = UIBaseControl:new{name = "Window", text = "Window text"}

function UIWindow:draw()
	putpng(self.xPos, self.yPos, "resources/window.png")
	puttext(self.xPos+13,self.yPos+8,self.text,1,1,1)
	puttext(self.xPos+14,self.yPos+7,self.text,255,255,255)
end