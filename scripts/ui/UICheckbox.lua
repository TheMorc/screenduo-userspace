--
-- checkbox control (derived from UIBaseControl)
--
UICheckbox = UIBaseControl:new{name = "Checkbox", text = "Checkbox"}
checked = false
enabled = true

function UICheckbox:draw()
	if self.checked then
		putpng(self.xPos, self.yPos, "resources/checkbox_checked.png")
	else
		putpng(self.xPos, self.yPos, "resources/checkbox_normal.png")
	end
end

function UICheckbox:pressEvent()
	UIBaseControl:pressEvent(self)
	self.checked = not self.checked
end
