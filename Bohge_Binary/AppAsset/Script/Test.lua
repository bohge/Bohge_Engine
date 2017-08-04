LUA_CLASS_NAME = String("Test");

Test = {}
Test.threadSysteam = 0
function Test.Initialize( timespan, framework )
	Test.threadSysteam = framework:GetPlugin( String("Thread") );
end


function Test.DoUpdate( timespan, framework )
	job = Test.threadSysteam:ThreadEntityFactory( IThreadSystem.ET_JOB_DISPATCHER );
	Test.threadSysteam:RecycleBin( job );
end


function Test.DoRender( timespan, framework )

end

function Test.Exit( timespan, framework )

end