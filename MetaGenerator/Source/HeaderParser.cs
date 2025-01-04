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

        public string Text => _headerInfo.text;
        private ref string HeaderText => ref _headerInfo.text;

        public HeaderParser(in HeaderInfo headerInfo)
        {
            _headerInfo = headerInfo;
        }

        public void Parse()
        {
            if (string.IsNullOrEmpty(HeaderText))
                return;

            RemoveAllComments(ref HeaderText);

            ParseNamespaces();
            ParseClasses();
            ParseEnums();

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

        private void ParseNamespaces() => _namespaceInfos = new NamespaceParser(HeaderText).Parse();

        private void ParseClasses() => _classInfos = new ClassParser(this).Parse();

        private void ParseEnums() => _enumInfos = new EnumParser(this).Parse();

        private static void RemoveAllComments(ref string data)
        {
            data = Regex.Replace(data, @"\/{2,}.*", string.Empty);
            data = Regex.Replace(data, @"\/\*[\s\S]*?\*\/", string.Empty);
        }
    }
}
