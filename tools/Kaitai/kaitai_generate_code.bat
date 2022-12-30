echo GENERATING GUCEF STRUCTURE FORMAT CODE
call kaitai-struct-compiler ./gucef_core_dynamic.ksy ./gucef_core_variant.ksy -t csharp -d ./ --dotnet-namespace GUCEF.CORE
call kaitai-struct-compiler ./gucef_pubsub_msgcontainer_header.ksy ./gucef_pubsub_msgcontainer_footer.ksy ./gucef_pubsub_msgcontainer.ksy -t csharp -d ./ --dotnet-namespace GUCEF.PUBSUB