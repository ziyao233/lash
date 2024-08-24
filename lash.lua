-- SPDX-License-Identifier: MIT
--[[
--	lash
--	lash.lua
--	Copyright (c) 2024 Yao Zi <ziyao at disroot dot org>
--]]

local raw		= require "lash.raw";

local function
glob(pattern)
	local r = raw.glob(pattern);
	local i = 0;
	return function()
		i = i + 1;
		return r[i];
	end;
end

local operations = {
	glob	= glob,
	exec	= raw.exec,
};

local function
getCmdName(s)
	return s:sub(1, 1) == '_' and s:sub(-1, -1) == '_' and s:sub(2, -2);
end

local function
indexCommand(self, k)
	local cmd = getCmdName(k);
	return cmd and function(...)
		return raw.exec(cmd, ...);
	end or rawget(self, k);
end

local function
shellcmd(on)
	if on then
		local m = getmetatable(_G) or {};
		m.__index = indexCommand;
		setmetatable(_G, m);
	else
		local m = getmetatable(_G);
		if m then
			m.__index = nil;
		end
	end
end

local function
exportopt(on)
	if on then
		for k, v in pairs(operations) do
			_G[k] = v;
		end
	end
end

return {
	glob		= glob,
	exec		= raw.exec,
	shellcmd	= shellcmd,
	exportopt	= exportopt,
       };
