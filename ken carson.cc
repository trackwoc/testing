getgenv().CamlockSettings = {
    jumpOffset = 0.90,
    fallOffset = 1.59,
    groundThreshold = 3,
    prediction = 0.1247724521,
    smoothness = 1
}

local NotificationHolder = loadstring(game:HttpGet("https://rentry.co/senopos/raw", true))()
local Notification = loadstring(game:HttpGet("https://rentry.co/senopos2/raw", true))()

local Sigmaballs = Instance.new("ScreenGui")
local ImageButton = Instance.new("ImageButton")
local Ui2corner = Instance.new("UICorner")
local player = game.Players.LocalPlayer
local YourTarget = nil
local enabled = false

Sigmaballs.Name = "Sigmaballs"
Sigmaballs.Parent = game.CoreGui
Sigmaballs.ZIndexBehavior = Enum.ZIndexBehavior.Sibling
Sigmaballs.ResetOnSpawn = false

ImageButton.Name = "ImageButton"
ImageButton.Parent = Sigmaballs
ImageButton.Active = true
ImageButton.Draggable = true
ImageButton.BackgroundColor3 = Color3.fromRGB(20, 20, 20)
ImageButton.BackgroundTransparency = 0.350
ImageButton.Size = UDim2.new(0, 90, 0, 90)
ImageButton.Image = "rbxassetid://76589332297789"
ImageButton.Position = UDim2.new(0.5, -25, 0.5, -25)

Ui2corner.CornerRadius = UDim.new(0.2, 0)
Ui2corner.Parent = ImageButton

function SigmaPlayerOhio()
    local mouse = player:GetMouse()
    local closestPlayer
    local shortestDistance = math.huge
    local CC = game:GetService("Workspace").CurrentCamera
    local screenCenter = Vector2.new(CC.ViewportSize.X / 2, CC.ViewportSize.Y / 2)
    
    for i, v in pairs(game.Players:GetPlayers()) do
        if v ~= player and v.Character and v.Character:FindFirstChild("Humanoid") and v.Character.Humanoid.Health ~= 0 and v.Character:FindFirstChild("HumanoidRootPart") then
            local pos = CC:WorldToViewportPoint(v.Character.PrimaryPart.Position)
            local magnitude = (Vector2.new(pos.X, pos.Y) - screenCenter).magnitude
            
            if v.Character.HumanoidRootPart.Position.Y > getgenv().CamlockSettings.groundThreshold then
                if magnitude < shortestDistance then
                    closestPlayer = v
                    shortestDistance = magnitude
                end
            end
        end
    end
    return closestPlayer
end

function PredictPosition(target)
    local targetRoot = target.Character:FindFirstChild("HumanoidRootPart")
    if targetRoot then
        local velocity = targetRoot.Velocity
        local predictedPosition = targetRoot.Position + (velocity * getgenv().CamlockSettings.prediction)
        
        if velocity.Y > 0 then
            predictedPosition = predictedPosition + Vector3.new(0, getgenv().CamlockSettings.jumpOffset, 0)
        elseif velocity.Y < 0 then
            predictedPosition = predictedPosition + Vector3.new(0, getgenv().CamlockSettings.fallOffset, 0)
        end

        return predictedPosition
    end
    return targetRoot.Position
end

function LockOntoTarget()
    if YourTarget and YourTarget.Character and YourTarget.Character:FindFirstChild("HumanoidRootPart") then
        local CC = game:GetService("Workspace").CurrentCamera
        local predictedPosition = PredictPosition(YourTarget)

        if predictedPosition then
            local currentCFrame = CC.CFrame

            local newCFrame = CFrame.new(CC.CFrame.Position, predictedPosition)
            CC.CFrame = currentCFrame:Lerp(newCFrame, getgenv().CamlockSettings.smoothness)
        end
    end
end

ImageButton.MouseButton1Click:Connect(function()
    if enabled then
        ImageButton.Image = "rbxassetid://76589332297789"
        enabled = false
        YourTarget = nil

        Notification:Notify(
            {Title = "kencarson.cc", Description = "@luascripting Lock Disabled"},
            {OutlineColor = Color3.fromRGB(0, 23, 13), Time = 2, Type = "default"}
        )
    else
        YourTarget = SigmaPlayerOhio()
        if YourTarget then
            enabled = true
            ImageButton.Image = "rbxassetid://76589332297789"

            Notification:Notify(
                {Title = "kencarson.cc", Description = "@luascripting Target Locked: " .. tostring(YourTarget.Character.Humanoid.DisplayName)},
                {OutlineColor = Color3.fromRGB(0, 23, 13), Time = 2, Type = "default"}
            )
        end
    end
end)

game:GetService("RunService").RenderStepped:Connect(function()
    if enabled and YourTarget then
        LockOntoTarget()
    end
end)
