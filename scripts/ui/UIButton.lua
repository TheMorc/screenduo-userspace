--
-- button control (derived from UIBaseControl)
--
UIButton = UIBaseControl:new{name = "Button", text = "Button"}
enabled = true
focused = false

function UIButton:draw()
	if self.focused then	
		putpng(self.xPos, self.yPos, "resources/button_focus.png")
	else
		putpng(self.xPos, self.yPos, "resources/button.png")
	end
	puttextxl(self.xPos/2+3,self.yPos/2+2,self.text,1,1,1)
	--putcharxl(self.xPos/2, self.yPos/2, 48 , 1 ,1,1 )
end

function UIButton:pressEvent()
	UIBaseControl:pressEvent(self)
end

function UIButton:enterFocusEvent()
	UIBaseControl:enterFocusEvent(self)
	self.focused = true
end

function UIButton:exitFocusEvent()
	UIBaseControl:exitFocusEvent(self)
	self.focused = false
end
