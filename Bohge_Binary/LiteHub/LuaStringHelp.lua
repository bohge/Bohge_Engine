--获取路径
function stripfilename(filename)
	return string.match(filename, "(.+)/[^/]*%.%w+$") --*nix system
	--return string.match(filename, “(.+)\\[^\\]*%.%w+$”) — windows
end

--获取文件名
function strippath(filename)
	return string.match(filename, ".+/([^/]*%.%w+)$") -- *nix system
	--return string.match(filename, “.+\\([^\\]*%.%w+)$”) — *nix system
end

--去除扩展名
function stripextension(filename)
	local idx = filename:match(".+()%.%w+$")
	if(idx) then
		return filename:sub(1, idx-1)
	else
		return filename
	end
end

--获取扩展名
function getextension(filename)
	return filename:match(".+%.(%w+)$")
end