import pygame
import os
from random import randint
from abc import ABC

"""
controls:
click sidebar buildings to select them
click to place selected buildings on the grid
mines go on nodes
R to rotate buildings
T to flip conveyor turns
right click to deselect, delete buildings

resources: cu, sn, fe, c
cu + sn = bronze
fe + c = steel
cu + cu = wire
fe + wire = circuit
fe + fe = gear
bronze + gear = machine
circuit + steel = computer
c + machine = engine

layers:
    0: background
    1: nodes
    2: buildings
    3: parts
    4: selected ghost building
"""

class Request:
    def __init__(self, part: str, amt: int):
        self.part = part
        self.amt = amt
        recipes = {
            "bronze": "cu + sn",
            "steel": "fe + c",
            "wire": "cu + cu",
            "circuit": "fe + wire(cu+cu)",
            "gear": "fe + fe",
            "machine": "bronze(cu+sn) + gear(fe+fe)",
            "computer": "circuit(fe+wire(cu+cu)) + steel(fe+c)",
            "engine": "c + machine(bronze(cu+sn) + gear(fe+fe))"
            }
        self.recipe = recipes[self.part]

    def __str__(self):
        return f"Produce {self.amt} {self.part}(s) -- {self.recipe}"

    def __sub__(self, s):
        if self.amt - 1 <= 0:
            return new_request()
        self.amt -= s
        return self

    def turn_in_part(self, part):
        if self.part == part.name:
            self -= 1
        # TODO: delete the part object in main after this is called
        
def new_request():
    parts = ["bronze", "steel", "wire", "circuit", "gear", "machine", "computer", "engine"]
    amts = [5, 10, 15]
    return Request(parts[randint(0, 7)], amts[randint(0, 2)])
        

class GridObject(ABC):
    def __init__(self, name: str, pos_x: int = 0, pos_y: int = 0, direction: str = "down", input_direction: str = "", output_direction: str = "", orientation: str = "clockwise"):
        self.name = name
        self.pos_x = pos_x
        self.pos_y = pos_y
        self.pos = (pos_x, pos_y)
        self.direction = direction
        self.image = pygame.image.load(f'graphics/{name}.bmp').convert()
        self.rect = None


class Part(GridObject):
    def __init__(self, name: str, pos_x: int, pos_y: int, building_on: "Building"):
        super().__init__(name, pos_x, pos_y)
        self.next_pos_x = pos_x
        self.next_pos_y = pos_y
        self.building_on = building_on
        self.has_moved = False


class Building(GridObject):
    def __init__(self, name: str, pos_x: int = 0, pos_y: int = 0, direction: str = "down", input_direction: str = "", output_direction: str = ""):
        super().__init__(name, pos_x, pos_y, direction)
        self.input_direction = input_direction
        self.output_direction = output_direction
        self.is_empty = True
        if self.output_direction == "down": self.output_pos = (self.pos_x, self.pos_y+1)
        if self.output_direction == "left": self.output_pos = (self.pos_x-1, self.pos_y)
        if self.output_direction == "up": self.output_pos = (self.pos_x, self.pos_y-1)
        if self.output_direction == "right": self.output_pos = (self.pos_x+1, self.pos_y)
        if self.input_direction == "down": self.input_pos = (self.pos_x, self.pos_y+1)
        if self.input_direction == "left": self.input_pos = (self.pos_x-1, self.pos_y)
        if self.input_direction == "up": self.input_pos = (self.pos_x, self.pos_y-1)
        if self.input_direction == "right": self.input_pos = (self.pos_x+1, self.pos_y)

class Conveyor(Building):
    pass

class Mine(Building):
    def __init__(self, name: str, pos_x: int = 0, pos_y: int = 0, direction: str = "down", output_direction: str = ""):
        super().__init__(name, pos_x, pos_y, direction, None, output_direction)
        for node in layer1:
            if (self.pos_x, self.pos_y) == (node.pos_x, node.pos_y):
                self.mined_part = node.name.replace("_node", "")

class Combiner(Building):
    def __init__(self, name: str, pos_x: int = 0, pos_y: int = 0, direction: str = "down", input_direction: str = "", output_direction: str = ""):
        super().__init__(name, pos_x, pos_y, direction, input_direction, output_direction)
        if self.input_direction == "left right": self.input_pos = [(self.pos_x-1, self.pos_y), (self.pos_x+1, self.pos_y)]
        if self.input_direction == "up down": self.input_pos = [(self.pos_x, self.pos_y-1), (self.pos_x, self.pos_y+1)]
        self.combining1: "Part" = None
        self.combining2: "Part" = None

class ConveyorTurn(Conveyor):
    def __init__(self, name: str, pos_x: int = 0, pos_y: int = 0, direction: str = "down", input_direction: str = "", output_direction: str = "", orientation: str = "clockwise"):
        super().__init__(name, pos_x, pos_y, direction, input_direction, output_direction)
        self.orientation = orientation
        
class ConveyorSplitter(Conveyor):
    def __init__(self, name: str, pos_x: int = 0, pos_y: int = 0, direction: str = "down", input_direction: str = "", output_direction: str = ""):
        output_direction = output_direction.split(' ')[0]
        super().__init__(name, pos_x, pos_y, direction, input_direction, output_direction)


class Sidebar(Building):
    def __init__(self, name: str, pos_x: int = 0, pos_y: int = 0, direction: str = "down", input_direction: str = "", output_direction: str = "", orientation: str = "clockwise"):
        super().__init__(name, pos_x, pos_y, direction, input_direction, output_direction)
        self.orientation = orientation
        
    def select(self):
        # select this building for placement
        global selecting
        selecting = True
        global selected_building
        selected_building = self
        
    def rotate(self):
        # rotate selected ghost building clockwise
        directions = ["down", "right", "up", "left"]
        self.direction = directions[directions.index(self.direction)-1]
        self.image = pygame.transform.rotate(self.image, -90)
        if self.name == "conveyor":
            self.output_direction = self.direction
            self.input_direction = directions[directions.index(self.direction)-2]
        if self.name == "mine":
            self.output_direction = self.direction
        if self.name == "combiner":
            self.output_direction = self.direction
            if self.input_direction == "left right": self.input_direction = "up down"
            else: self.input_direction = "left right"
        if self.name == "conveyor_turn":
            self.output_direction = directions[directions.index(self.output_direction)-1]
            self.input_direction = directions[directions.index(self.input_direction)-1]
        if self.name == "conveyor_splitter":
            self.input_direction = directions[directions.index(self.input_direction)-1]
            if self.output_direction == "left right": self.output_direction = "up down"
            else: self.output_direction = "left right"

    def reorient(self):
        # flip selected conveyor_turn to the other orientation
        if self.orientation == "clockwise": self.orientation = "counterclockwise"
        if self.orientation == "counterclockwise": self.orientation = "clockwise"
        self.image = pygame.transform.flip(self.image, True, False)
        if self.input_direction == "left": self.input_direction = "right"
        elif self.input_direction == "right": self.input_direction = "left"
        if self.output_direction == "left":
            self.output_direction = "right"
            self.direction = "right"
        elif self.output_direction == "right":
            self.output_direction = "left"
            self.direction = "left"

    def check_place(self, x, y): 
        # check if building can be placed, if so then place it
        no_building = True
        no_node = True
        if (x, y) == (13, 7): # can't place on the output goal
            return
        for building in layer2: # can't overlap with other buildings
            if x == building.pos_x and y == building.pos_y:
                no_building = False
        for node in layer1:
            if x == node.pos_x and y == node.pos_y:
                no_node = False
        if no_building and not no_node: # only mines on nodes
            if selected_building.name == "mine":
                self.place(x, y)
        if no_building and no_node: # anything else can go anywhere else
            if selected_building.name != "mine":
                self.place(x, y)

    def place(self, x, y):
        # place a selected building on the grid
        if self.name == "conveyor": layer2.append(Conveyor(self.name, x, y, self.direction, self.input_direction, self.output_direction))
        if self.name == "mine": layer2.append(Mine(self.name, x, y, self.direction, self.output_direction))
        if self.name == "combiner": layer2.append(Combiner(self.name, x, y, self.direction, self.input_direction, self.output_direction))
        if self.name == "conveyor_turn": layer2.append(ConveyorTurn(self.name, x, y, self.direction, self.input_direction, self.output_direction, self.orientation))
        if self.name == "conveyor_splitter": layer2.append(ConveyorSplitter(self.name, x, y, self.direction, self.input_direction, self.output_direction))
        layer2[-1].image = self.image.copy()
        


class ResourceNode(GridObject):
    pass
   


def main():
    # pygame setup
    pygame.init()
    screen = pygame.display.set_mode((1280, 720))
    clock = pygame.time.Clock()
    running = True
    time = 1

    global layer1
    layer1 = []
    global layer2
    layer2 = []
    global layer3
    layer3 = []

    recipes = {
        ("cu", "sn"): "bronze",
        ("fe", "c"): "steel",
        ("cu", "cu"): "wire",
        ("fe", "wire"): "circuit",
        ("fe", "fe"): "gear",
        ("bronze", "gear"): "machine",
        ("circuit", "steel"): "computer",
        ("c", "machine"): "engine"
        }

    background = pygame.image.load('graphics/background.bmp').convert()

    # initialize selectable sidebar buildings
    sb_conveyor = Sidebar("conveyor", 65, 62, "down", "up", "down")
    sb_mine = Sidebar("mine", 65, 179, "down", "", "down")
    sb_combiner = Sidebar("combiner", 65, 299, "down", "left right", "down")
    sb_conveyor_turn = Sidebar("conveyor_turn", 65, 422, "down", "left", "down", "clockwise")
    sb_conveyor_splitter = Sidebar("conveyor_splitter", 65, 546, "down", "up", "left right")
    global sidebar
    sidebar = [sb_conveyor, sb_mine, sb_combiner, sb_conveyor_turn, sb_conveyor_splitter]
    global selecting
    selecting = False
    global selected_building
    selected_building = None

    # initialize resource nodes
    rn_cu0 = ResourceNode("cu_node", 0, 0)
    rn_cu1 = ResourceNode("cu_node", 1, 0)
    rn_sn0 = ResourceNode("sn_node", 0, 3)
    rn_sn1 = ResourceNode("sn_node", 0, 4)
    rn_fe0 = ResourceNode("fe_node", 5, 0)
    rn_fe1 = ResourceNode("fe_node", 6, 0)
    rn_c0 = ResourceNode("c_node", 10, 0)
    rn_c1 = ResourceNode("c_node", 11, 0)
    layer1 += [rn_cu0, rn_cu1, rn_sn0, rn_sn1, rn_fe0, rn_fe1, rn_c0, rn_c1]
    
    # set initial request    
    request = Request("bronze", 5)

    while running:
        mouse_x, mouse_y = pygame.mouse.get_pos()
        # poll for events
        for event in pygame.event.get():            
            if event.type == pygame.QUIT: # pygame window is closed
                running = False
            if event.type == pygame.MOUSEBUTTONDOWN and event.button == 1: # left click
                # check if anything is being clicked, do the relevant action
                if selecting:
                    if 216 <= mouse_x <= 1254 and 112 <= mouse_y <= 706: # clicked inside grid
                        # convert pixel coordinates to in-game grid coordinates
                        coord_x = (mouse_x - 216) // 76
                        coord_y = (mouse_y - 112) // 76
                        selected_building.check_place(coord_x, coord_y)
                        
                for sb in sidebar:
                    if sb.rect.collidepoint((mouse_x, mouse_y)): # sidebar building gets clicked
                        sb.select()
            if event.type == pygame.MOUSEBUTTONDOWN and event.button == 3: # right click
                selecting = False
                for building in layer2:
                    if building.pos_x == ((mouse_x - 216) // 76) and building.pos_y == ((mouse_y - 112) // 76):
                        layer2.remove(building)
                for part in layer3:
                    if part.pos_x == ((mouse_x - 216) // 76) and part.pos_y == ((mouse_y - 112) // 76):
                        layer3.remove(part)
            if event.type == pygame.KEYDOWN and event.key == pygame.K_r: # R key pressed
                if selecting:
                    selected_building.rotate()
            if event.type == pygame.KEYDOWN and event.key == pygame.K_t: # T key pressed
                if selecting and selected_building.name == "conveyor_turn":
                    selected_building.reorient()


        if time % 60 == 0:
            # move all parts, check for mining every 60 frames/1 second
            for part in layer3: part.has_moved = False
            for b in layer2: # TODO: check for turning in
                for part in layer3:
                    
                    # turn in parts
                    if part.building_on.output_pos == (13, 7):
                        if part.name == request.part:
                            part.building_on.is_empty = True
                            layer3.remove(part)
                            request -= 1

                    # move parts
                    if part.building_on.output_pos == b.pos and b.input_pos == part.building_on.pos and b.is_empty and not part.has_moved:
                        if b.name == "conveyor" or b.name == "conveyor_turn" or b.name == "conveyor_splitter":
                            if part.building_on.name == "conveyor_splitter":
                                # switch splitter direction every output
                                if part.building_on.output_direction == "left":
                                    part.building_on.output_pos = (part.building_on.pos_x+1, part.building_on.pos_y)
                                    part.building_on.output_direction = "right"
                                elif part.building_on.output_direction == "right":
                                    part.building_on.output_pos = (part.building_on.pos_x-1, part.building_on.pos_y)
                                    part.building_on.output_direction = "left"
                                elif part.building_on.output_direction == "up":
                                    part.building_on.output_pos = (part.building_on.pos_x, part.building_on.pos_y+1)
                                    part.building_on.output_direction = "down"
                                elif part.building_on.output_direction == "down":
                                    part.building_on.output_pos = (part.building_on.pos_x, part.building_on.pos_y-1)
                                    part.building_on.output_direction = "up"
                            part.building_on.is_empty = True
                            part.building_on = b
                            part.pos_x, part.pos_y = b.pos
                            part.pos = b.pos
                            part.has_moved = True
                            b.is_empty = False
                            continue
                        
                    # combine parts
                    if b.name == "combiner" and part.building_on.output_pos == b.pos:
                        if b.input_pos[0] == part.pos: b.combining1 = part
                        elif b.input_pos[1] == part.pos: b.combining2 = part
                        if b.combining1 != None and b.combining2 != None and b.combining1 != b.combining2 and b.combining1 in layer3 and b.combining2 in layer3 and b.is_empty:
                            if (b.combining1.name, b.combining2.name) in recipes.keys() or (b.combining2.name, b.combining1.name) in recipes.keys():
                                b.combining1.building_on.is_empty = True
                                b.combining2.building_on.is_empty = True
                                b.is_empty = False
                                layer3.remove(b.combining1)
                                layer3.remove(b.combining2)
                                try:
                                    combined_part = recipes[(b.combining1.name, b.combining2.name)]
                                except:
                                    combined_part = recipes[(b.combining2.name, b.combining1.name)]
                                layer3.append(Part(combined_part, b.pos_x, b.pos_y, b))
                                b.combining1 = None
                                b.combining2 = None

                # create parts from mines
                if b.name == "mine":
                    for c in layer2:
                        if b.output_pos == c.pos and c.input_pos == b.pos:
                            overlap_part = False
                            for p in layer3:
                                if b.output_pos == p.pos:
                                    overlap_part = True
                            if not overlap_part:
                                layer3.append(Part(b.mined_part, c.pos_x, c.pos_y, c))
                                c.is_empty = False
        

        # RENDERING
        
        # update the background FIRST (layer 0)
        screen.blit(background, (0,0))

        # update the request line
        font = pygame.font.Font(None, 40)
        text = font.render(str(request), True, (10, 10, 10))
        screen.blit(text, (240, 40))
        
        # update layers
        for sprite in sidebar: # sidebar buildings aren't on the grid, they use pixel coordinates
            sprite.rect = screen.blit(sprite.image, sprite.pos)
        for layer in [layer1, layer2]:   
            for sprite in layer: # convert grid coordinates to pixel coordinates, render
                sprite.rect = screen.blit(sprite.image, (216 + sprite.pos_x*74, 112 + sprite.pos_y*74))
        for sprite in layer3: # render the smaller parts in the middle of the cells
            sprite.rect = screen.blit(sprite.image, (236 + sprite.pos_x*74, 132 + sprite.pos_y*74))

        # render selected ghost building LAST
        if selecting:
            screen.blit(selected_building.image, (mouse_x-34, mouse_y-34))

        # flip() the display to put your work on screen
        pygame.display.flip()
        

        clock.tick(60)  # limits FPS to 60
        time += 1
        

    pygame.quit()

if __name__ == "__main__":
    main()
