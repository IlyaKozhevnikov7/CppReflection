using PCRE;
using System.Collections.Generic;
using System.Linq;
using System.Text.RegularExpressions;

namespace MetaGenerator
{
    public class ClassParser : PrimaryMemberParser
    {
        public ClassParser(HeaderParser headerParser) : base(headerParser)
        {
        }

        public ClassInfo[] Parse()
        {
            IEnumerable<PcreMatch> matches = FindMatches(@"REFLECTABLE(?<attr>\((?:[^()]|(?&attr))*\))\s*(?<templ>\btemplate\b<(?:[^<>]|(?&templ))*>)?\s*(?<!\benum\b)(class|struct)\b[\s\S]*?(?<body>\{(?:[^{}]|(?&body))*\});", Text);
            return ParseClassInfos(matches);
        }

        private ClassInfo[] ParseClassInfos(IEnumerable<PcreMatch> matches)
        {
            var classInfos = MemberInfoBase.Construct<ClassInfo>(matches.Count());

            int i = 0;
            foreach (var match in matches)
            {
                var bodyGroup = match["body"];
                int headerStart = match["attr"].EndIndex;
                string header = Text.Substring(headerStart, bodyGroup.Index - headerStart);

                ParseClassHeader(header, ref classInfos[i]);
                classInfos[i].namespaceName = HeaderParser.GetNamespaceByPosition(match.Index);
                classInfos[i].fields = ParseFieldInfos(bodyGroup.Value);
                classInfos[i].methods = ParseMethodInfos(bodyGroup.Value);
                classInfos[i].constructors = ParseConstructorInfos(bodyGroup.Value, classInfos[i].name);

                var templMatch = match["templ"];
                classInfos[i].templateParameters = templMatch.Success ? templMatch.Value : null;

                ++i;
            }

            return classInfos;
        }

        private void ParseClassHeader(string header, ref ClassInfo info)
        {
            int parentsStart = header.IndexOf(':');
            bool hasParents = parentsStart != -1;

            string nameSubString = hasParents
                ? header.Substring(0, parentsStart)
                : header;

            info.name = ExcludeClassName(nameSubString, out info.isFinal);
            info.instanceOf = ExcludeInstanceOf(nameSubString);

            info.parentNames = hasParents
                ? ExcludeParents(header.Substring(parentsStart + 1, header.Length - parentsStart - 1))
                : [];
        }

        private string ExcludeClassName(string subString, out bool isFinal)
        {
            var matches = Regex.Matches(subString, @"\S+");

            isFinal = matches[matches.Count - 1].Value == "final";

            return isFinal
                ? matches[matches.Count - 2].Value
                : matches[matches.Count - 1].Value;
        }

        private InstanceOf ExcludeInstanceOf(string subString)
        {
            return Regex.IsMatch(subString, @"\bclass\b")
                ? InstanceOf.Class
                : InstanceOf.Struct;
        }

        private string[] ExcludeParents(string subString)
        {
            var matches = FindMatches(@"[\S\s]*?(\S+?(?<args>[<(]+?(?:[^<>()]|(?&args))*[>)]+?)?)\s*(,|$)", subString);
            int count = matches.Count();

            string[] parents = new string[count];
            int i = 0;
            foreach (var match in matches)
            {
                parents[i] = match[1];
                ++i;
            }

            return parents;
        }

        private FieldInfo[] ParseFieldInfos(string body) => new FieldParser(body).Parse();
        private ConstructorInfo[] ParseConstructorInfos(string body, string className) => new ConstructorParser(body).Parse(className);
        private MethodInfo[] ParseMethodInfos(string body) => new MethodParser(body).Parse();
    }
}
