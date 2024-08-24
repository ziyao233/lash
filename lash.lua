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

return {
	glob		= glob,
       };
