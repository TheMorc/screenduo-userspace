--
-- checkbox control (derived from UIBaseControl)
--
UIRadiobutton = UIBaseControl:new{name = "Radiobutton", text = "Radiobutton"}
selected = false
enabled = true

function UIRadiobutton:draw()
	if self.selected then
		putpng(self.xPos, self.yPos, "resources/radiobutton_selected.png")
	else
		putpng(self.xPos, self.yPos, "resources/radiobutton_normal.png")
	end
end

function UIRadiobutton:pressEvent()
	UIBaseControl:pressEvent(self)
	
	self.selected = not self.selected
end
