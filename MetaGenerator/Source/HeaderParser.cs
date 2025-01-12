using System.Text;
using System.Text.RegularExpressions;

namespace MetaGenerator
{
    public class HeaderParser
    {
        public FileInfo? FileInfo
        {
            get
            {
                if (_classInfos.Length == 0 && _enumInfos.Length == 0)
                    return null;

                return new FileInfo(_headerInfo.path, _classInfos, _enumInfos);
            }
        }

        private HeaderInfo _headerInfo;
        private ClassInfo[] _classInfos = null;
        private EnumInfo[] _enumInfos = null;
        private NamespaceInfo[] _namespaceInfos = null;

        private ref string TextRef => ref _headerInfo.text;

        public string Text => _headerInfo.text;

        public HeaderParser(in HeaderInfo headerInfo)
        {
            _headerInfo = headerInfo;
        }

        public void Parse()
        {
            if (string.IsNullOrEmpty(TextRef))
                return;

            RemoveAllComments();

            _namespaceInfos = ParseNamespaces();
            _classInfos = ParseClasses();
            _enumInfos = ParseEnums();

            ProjectData.RegisterClasses(_classInfos);
            ProjectData.RegisterEnums(_enumInfos);
        }

        public string GetNamespaceByPosition(int position)
        {
            StringBuilder builder = new();

            foreach (var namespaceInfo in _namespaceInfos)
            {
                if (namespaceInfo.IsBound(position))
                {
                    if (builder.Length > 0)
                        builder.Append("::");
                    
                    builder.Append(namespaceInfo.Name);
                    continue;
                }
                
                if (builder.Length > 0)
                    return builder.ToString();
            }

            return builder.Length > 0 
                ? builder.ToString()
                : null;
        }

        private NamespaceInfo[] ParseNamespaces() => new NamespaceParser(this).Parse();

        private ClassInfo[] ParseClasses() => new ClassParser(this).Parse();

        private EnumInfo[] ParseEnums() => new EnumParser(this).Parse();

        private void RemoveAllComments()
        {
            TextRef = Regex.Replace(TextRef, @"\/{2,}.*", string.Empty);
            TextRef = Regex.Replace(TextRef, @"\/\*[\s\S]*?\*\/", string.Empty);
        }
    }
}
