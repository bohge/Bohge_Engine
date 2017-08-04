--生命一个全局的类别
meta = {}
meta.data = Float(567);
--后面的函数和常量定义都无需模块限定符了。
function meta.main()
	Iplugin = framework:GetPlugin( Framework.FC_IO );
	Iplugin:Func();
	
	Iplugin = framework:GetPlugin( Framework.FC_SCRIPT );
	Iplugin:Func();
end


--返回和模块对应的table。
--[[
				Set = {}
				
				function Set.add(a,b)
					local res = Set.new{}
					res[1] = a[1]+b[1]
					res[2] = a[2]+b[2]
					return res
				end
				
				function Set.printSet (s)
					print( s[1] )
					print( s[2] )
				end
				
				mt= {}
				mt.__add = Set.add;
				
				function Set.new(l)
					local set = {}
					setmetatable(set,mt)
					--for _, v in ipairs(l) do set[v] = true end
					set[1] = l[1];
					set[2] = l[2];
					return set
				end
				
				function main()
					local s1 = Set.new{1,2}
					local s2 = Set.new{4,5}
					s3 = s1 + s2
					Set.printSet( s3 )
					vec1 = vector2f(Float(11),Float(1));
					vec2 = vector2f(Float(22),Float(2));
					vec3 = vector2f(Float(33),Float(3));
					--vec3 = vec1:AddVector(vec2);
					--vec3 = vec1 + vec2;
					--f1 = vec3:Dot(vec1);
					--vec3 = vec1 * f1;
					--vec3 = vec1 * vec2;
					vec3 = vec1 * vecTest;
					--vec1 = vec3 * vec2;
					vec2 = vec3 * Float(10);
					base = Base()
					base:Func();
					driver = Driver();
					driver:Func();
					--mat22 = matrix22f(1,2,3,4);
					--vec3 = vec2 * mat22;
					--vec3:TestFunc( vec1 );
					--vec3:TestFunc( vec1,vec1 );
					--vec3:TestFunc( 1,2,3,4,5,6 );
				end
				]]--