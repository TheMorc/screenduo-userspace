--
-- checkbox control (derived from UIBaseControl)
--
UIProgressbar = UIBaseControl:new{name = "Progress Bar", text = "Progress Bar"}
enabled = true
value = 0
maxValue = 100
width = 50

function UIProgressbar:draw()
		putpng(self.xPos, self.yPos, "resources/progressbar_corner.png") 
		putpng(self.xPos + self.width+1, self.yPos, "resources/progressbar_corner.png") 
		
		for pbarWidth = 1, self.width
		do
			--print("Hi"..self.value)
			putpng(self.xPos+pbarWidth, self.yPos, "resources/progressbar_normalbar.png")
		end
		
		
		for pbarValue = 1, remap(self.value,0,self.maxValue,0,self.width)
		do
			--print("Hi"..self.value)
			putpng(self.xPos+pbarValue, self.yPos, "resources/progressbar_bar.png")
		end
end
