--
-- checkbox control (derived from UIBaseControl)
--
UIProgressbar = UIBaseControl:new{name = "Progress Bar", text = "Progress Bar"}
enabled = true
value = 0

function UIProgressbar:draw()
		putpng(self.xPos, self.yPos, "resources/progressbar.png")
		for pbarValue = 1, self.value
		do
			--print("Hi"..self.value)
			putpng(self.xPos+1+pbarValue, self.yPos, "resources/progressbar_bar.png")
		end
end
