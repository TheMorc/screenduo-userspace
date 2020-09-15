--Simple snake example game
--heavily modified port of https://github.com/elua/snake/blob/master/snake.lua
snake = {};
app_name = "Snake"
app_desc = "Snake game"
app_version = 1.1
app_icon = "apps/snake.png"

local xMax = math.floor( 320 / 6 ) - 1
local yMax = math.floor( 240 / 8 ) - 1
local game_map = {}

local Head = {}
local Tail = {}

local highscore = 0
local size = 1
Tail.x = 1
Tail.y = 1
Head.x = Tail.x + ( size - 1 )
Head.y = Tail.y

local Food = {}
Food.x = false
Food.y = false

Head.dx = 1
Head.dy = 0
Tail.dx = Head.dx
Tail.dy = Head.dy
local direction = "right"
local level = 1
local score = 0



local function create_food()
-- if not food then
  Food.x, Food.y = math.random( xMax - 1), math.random( yMax - 1)
  while game_map[ Food.x ][ Food.y ] do
    Food.x, Food.y = math.random( xMax - 1 ), math.random( yMax - 1 )
  end
  game_map[ Food.x ][ Food.y ] = "food"
  putpixel( Food.x * 6, Food.y * 8, 0,255,0 )
-- end
end

local function eat_food()
  putpixel(Head.x * 6, Head.y * 8, 0,0,0 )
  game_map[ Head.x ][ Head.y ] = nil
  create_food()
  score = score + level
end

local function check_collision()
  if Head.x <= 0 or Head.x >= xMax then
    return true
  elseif Head.y <= 0 or Head.y >= yMax then
    return true
  elseif ( ( game_map[ Head.x ][ Head.y ] ) and ( game_map[ Head.x ][ Head.y ] ~= "food" ) ) then
    return true
  end
  return false
end

local function move()
  if game_map[ Tail.x ][ Tail.y ] == "right" then
    Tail.dx = 1
    Tail.dy = 0
  elseif game_map[ Tail.x ][ Tail.y ] == "left" then
    Tail.dx = -1
    Tail.dy = 0
  elseif game_map[ Tail.x ][ Tail.y ] == "up" then
    Tail.dx = 0
    Tail.dy = -1
  elseif game_map[ Tail.x ][ Tail.y ] == "down" then
    Tail.dx = 0
    Tail.dy = 1
  end
    game_map[ Head.x ][ Head.y ] = direction
    Head.x = Head.x + Head.dx
    Head.y = Head.y + Head.dy

  if game_map[ Head.x ][ Head.y ] == "food" then
    eat_food()
  else
    putpixel( Tail.x * 6, Tail.y * 8,  0,0,0)
    game_map[ Tail.x ][ Tail.y ] = nil
    Tail.x = Tail.x + Tail.dx
    Tail.y = Tail.y + Tail.dy
  end

  putpixel( Head.x * 6, Head.y * 8,  255,255,255 )
end

local function draw_walls()
  for i = 0, xMax*2, 1 do
    putpixel(i * 3, yMax * 8,  255,255,255 )
    putpixel(i * 3+1, yMax * 8,  255,255,255 )
    putpixel(i * 3+3, yMax * 8,  255,255,255 )
    putpixel(i * 3+4, yMax * 8,  255,255,255 )
    putpixel(i * 3+5, yMax * 8,  255,255,255 )
    putpixel(i * 3, 0, 255,255,255 )
    putpixel(i * 3+1, 0, 255,255,255 )
    putpixel(i * 3+2, 0, 255,255,255 )
    putpixel(i * 3+3, 0, 255,255,255 )
  end
  for i = 0, yMax*2, 1 do
    putpixel(xMax * 6, i * 4,  255,255,255 )
    putpixel(xMax * 6, i * 4+1,  255,255,255 )
    putpixel(xMax * 6, i * 4+2,  255,255,255 )
    putpixel(xMax * 6, i * 4+3,  255,255,255 )
    putpixel(0, i * 4, 255,255,255 )
    putpixel(0, i * 4+1, 255,255,255 )
    putpixel(0, i * 4+2, 255,255,255 )
    putpixel(0, i * 4+3, 255,255,255 )
  end
end


function snake:init()

  clearscreen()
  food = false
  running = true
  draw_walls()
  size = 5
  score = 0
  level = 1
  Tail.x = 1
  Tail.y = 1
  Head.x = Tail.x + ( size - 1 )
  Head.y = Tail.y
  Head.dx = 1
  Head.dy = 0
  Tail.dx = Head.dx
  Tail.dy = Head.dy
  direction = "right"

  for i = 0, xMax, 1 do
    game_map[ i ] = {}
  end
  for i = 0, size - 1, 1 do
    game_map[ Tail.x + ( i * Tail.dx ) ][ Tail.y + ( i * Tail.dy ) ] = direction
    putpixel(( Tail.x + ( i * Tail.dx ) ) * 6, ( Tail.y + ( i * Tail.dy ) ) * 8, 255,255,255 ) --args: x, y, r, g, b color(0-255)
  end
  create_food()
end

function snake:update()
	if running == true then
    	direction = dir
    	move()
    end


	if check_collision() then
		running = false
		if score > highscore then
    		highscore = score
  		end
  		print("Game over :(")
  		print("Your score was "..tostring(score))
  		print("Highscore: "..tostring(highscore))
  		--print( "SELECT to restart" )
		app_close()
	end
	
	render(false)
end

function snake:buttonPress(btn)
    if btn == 2 then dir = "right"   Head.dx = 1   Head.dy = 0
    elseif btn == 1 then  dir = "left"   Head.dx = -1  Head.dy = 0
    elseif btn == 3 then dir = "up"   Head.dx = 0   Head.dy = -1
    elseif btn == 4 then dir = "down"   Head.dx = 0   Head.dy = 1
    elseif btn == 0 then print(btn .. " (enter)") level = level + 1
    elseif btn == 6 then app_close()
    end
end

function snake:getName()
	return app_name
end

function snake:getDesc()
	return app_desc
end

function snake:getVersion()
	return app_version
end

function snake:getIcon()
	return app_icon
end

return snake